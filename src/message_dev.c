#include "message_dev.h"

#include "kmessage.h"
#include "message_limit.h"

#include <linux/kernel.h>
#include <linux/errno.h>

int kmessaged_message_dev_init(struct kmessaged_message_dev_t *mdev)
{
    msglmt_t msglmt;

    if (!mdev) {
        printk(KERN_DEBUG "kmessaged: null mdev supplied to kmessaged_message_dev_init\n");

        return EINVAL;
    }

    msglmt = kmessaged_get_message_limit();

    mdev->unread_msgs = kmalloc(sizeof(struct kmessage_t) * msglmt, GFP_KERNEL);

    if (!mdev->unread_msgs) {
        return ENOMEM;
    }

    mdev->read_msgs = kmalloc(sizeof(struct kmessage_t) * KMESSAGED_DEFAULT_TOTAL_MESSAGE_LIMIT, GFP_KERNEL);

    if (!mdev->read_msgs) {
        return ENOMEM;
    }

    mdev->unread_cnt = 0UL;
    mdev->read_cnt = 0UL;
    mdev->resize_cnt = 0UL;
    mdev->rdmod = EXCLUDE_READ;
    mdev->msglmt = KMESSAGED_DEFAULT_MESSAGE_LIMIT;
    memset(&mdev->targets, 0, sizeof(struct kmessaged_message_dev_target_t) * 255);
    mdev->target_cnt = 0;
    sema_init(&mdev->target_dsema, 1);

    return 0;
}

int kmessaged_message_dev_add(struct kmessaged_message_dev_t *mdev, const struct kmessage_t *msg)
{
    msglmt_t msglmt;

    //  TODO: Check against memory boundaries, resize if needed.
    
    msglmt = kmessaged_get_message_limit();

    if (mdev->unread_cnt >= msglmt) {
        return ENOMEM;
    }

    memcpy(&mdev->unread_msgs[mdev->unread_cnt], msg, sizeof(struct kmessage_t));
    mdev->unread_cnt += 1;

    printk(KERN_NOTICE "kmessaged: message copied to mdev, new message count %lu\n", mdev->unread_cnt);

    return 0;
}

int kmessaged_message_dev_readall(struct kmessaged_message_dev_t *mdev)
{
    msglmt_t msglmt;

    msglmt = kmessaged_get_message_limit();

    memcpy(&mdev->read_msgs[mdev->read_cnt], mdev->unread_msgs, sizeof(struct kmessage_t) * mdev->unread_cnt);
    memset(mdev->unread_msgs, 0, sizeof(struct kmessage_t) * msglmt);

    mdev->read_cnt += mdev->unread_cnt;
    mdev->unread_cnt = 0;

    return 0;
}

int kmessaged_message_dev_expand(struct kmessaged_message_dev_t *mdev)
{
    msglmt_t msglmt;
    struct kmessage_t *tmp;

    msglmt = kmessaged_get_message_limit();

    mdev->resize_cnt += 1;

    tmp = kmalloc(sizeof(struct kmessage_t) * (mdev->resize_cnt + 1) * KMESSAGED_DEFAULT_TOTAL_MESSAGE_LIMIT, GFP_KERNEL);

    if (!tmp) {
        printk(KERN_DEBUG "kmessaged: no memory left to expand message dev on size %lu\n", (mdev->resize_cnt + 1) * KMESSAGED_DEFAULT_TOTAL_MESSAGE_LIMIT);

        return ENOMEM;
    }

    memcpy(tmp, mdev->read_msgs, sizeof(struct kmessage_t) * mdev->read_cnt);
    
    kfree(mdev->read_msgs);

    mdev->read_msgs = tmp;

    return 0;
}

int kmessaged_message_dev_add_target(struct kmessaged_message_dev_t *mdev, const char *uname, const uid_t uid)
{
    struct kmessaged_message_dev_target_t *tg;

    if (down_interruptible(&mdev->target_dsema)) {
        return -ERESTARTSYS;
    }

    tg = &mdev->targets[mdev->target_cnt];

    tg->name = uname;
    tg->uid = uid;

    mdev->target_cnt += 1;

    up(&mdev->target_dsema);

    return 0;
}

uid_t kmessaged_message_dev_find_target(struct kmessaged_message_dev_t *mdev, const char *uname)
{
    ssize_t i;

    for (i = 0; i < mdev->target_cnt; ++i) {
        if (strcmp(mdev->targets[i].name, uname) == 0) {
            return mdev->targets[i].uid;
        }
    }

    return -1;
}

int kmessaged_message_dev_resolve_target(struct kmessaged_message_dev_t *mdev, const char *uname, const uid_t uid)
{
    ssize_t i;

    for (i = 0; i < mdev->target_cnt; ++i) {
        if (mdev->targets[i].uid == uid) {
            strcpy(uname, mdev->targets[i].name);

            return 0;
        }
    }

    return ENOENT;
}

int kmessaged_message_dev_release(struct kmessaged_message_dev_t *mdev)
{
    ssize_t i;

    if (!mdev) {
        return EINVAL;
    }

    kfree(mdev->unread_msgs);
    kfree(mdev->read_msgs);

    for (i = 0; i < mdev->target_cnt; ++i) {
        kfree(mdev->targets[i].name);
    }

    return 0;
}


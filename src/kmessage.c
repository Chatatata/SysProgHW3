#include "kmessage.h"

#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/string.h>

int kmessaged_msg_init(const kmessage_t *msgstruct, const unsigned long len, const uid_t uid)
{
    if (!msgstruct || len == 0) 
        return EINVAL;

    msgstruct->msg = kmalloc(sizeof(char) * len, GFP_KERNEL);

    if (!msgstruct->msg)
        return ENOMEM;

    msgstruct->len = len;
    msgstruct->uid = uid;

    return 0;
}

int kmessaged_msg_create(const kmessage_t *msgstruct, const char *msgdata, const uid_t uid)
{
    size_t len;
    
    if (!msgdata) 
        return EINVAL;

    len = strlen(msgdata);

    msgstruct->msg = kmalloc(len * sizeof(char), GFP_KERNEL);

    if (!msgstruct->msg)
        return ENOMEM;

    memcpy(msgstruct->msg, msgdata, len * sizeof(char));
    msgstruct->len = len;
    msgstruct->uid = uid;

    return 0;
}

int kmessaged_msg_release(const kmessage_t *msgstruct)
{
    kfree(msgstruct->msg);

    return 0;
}

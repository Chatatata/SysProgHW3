#include "kmessage.h"

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/string.h>

int kmessaged_msg_create(struct kmessage_t *msgstruct, const char *msgdata, const uid_t uid, const char *recipient)
{
    size_t len;
    
    if (!msgdata || !recipient) {
        printk(KERN_DEBUG "kmessaged: msgdata %x or recipient %x was NULL\n", msgdata, recipient);

        return EINVAL;
    }
    
    msgstruct->msg = msgdata;
    msgstruct->len = len;
    msgstruct->uid = uid;
    msgstruct->recipient = recipient;

    return 0;
}

int kmessaged_msg_release(struct kmessage_t *msgstruct)
{
    kfree(msgstruct->msg);
    kfree(msgstruct->recipient);

    return 0;
}


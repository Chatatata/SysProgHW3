#include "open_release.h"

#include "message_dev.h"

int kmessaged_open(struct inode *inode, struct file *filp)
{
    struct kmessaged_message_dev_t *dev;
    int result;

    dev = container_of(inode->i_cdev, struct kmessaged_message_dev_t, cdev);
    filp->private_data = dev;

    return result;
}

int kmessaged_release(struct inode *inode, struct file *filp)
{
    struct kmessaged_message_dev_t *dev;
    int result;

    dev = filp->private_data;

    return result;
}

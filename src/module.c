#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/proc_fs.h>
#include <linux/fcntl.h>
#include <linux/seq_file.h>
#include <linux/cdev.h>
#include <asm/switch_to.h>
#include <asm/uaccess.h>

#include "kmessaged.h"
#include "message_dev.h"
#include "rw.h"
#include "open_release.h"
#include "ioctl.h"
#include "llseek.h"

unsigned int kmessaged_dev_major = KMESSAGED_DEFAULT_DEV_MAJOR;
unsigned int kmessaged_dev_minor = KMESSAGED_DEFAULT_DEV_MINOR;

module_param(kmessaged_dev_major, int, S_IRUGO);
module_param(kmessaged_dev_minor, int, S_IRUGO);

MODULE_AUTHOR("Bugra Ekuklu, Muratcan Sahin");
MODULE_DESCRIPTION("Kernel message daemon as a character device amongst OS users.");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_VERSION("0.1");

static struct kmessaged_message_dev_t *mdev;

struct file_operations kmessaged_ops = {
    .owner = THIS_MODULE,
    .llseek = kmessaged_llseek,
    .read = kmessaged_read,
    .write = kmessaged_write,
    .unlocked_ioctl = kmessaged_ioctl,
    .open = kmessaged_open,
    .release = kmessaged_release
};

/**
 * kmessaged_init
 *
 * Initializes the `kmessaged` module.
 *
 * @returns Returns 0 if succesful.
 */
static int __init kmessaged_init(void)
{
    int result = 0;
    dev_t devno = 0;

    if (KMESSAGED_DEFAULT_DEV_MAJOR) {
        devno = MKDEV(KMESSAGED_DEFAULT_DEV_MAJOR, KMESSAGED_DEFAULT_DEV_MINOR);
        result = register_chrdev_region(devno, 1, KMESSAGED_DV_NAME);
    } else {
        result = alloc_chrdev_region(&devno, KMESSAGED_DEFAULT_DEV_MINOR, 1, KMESSAGED_DV_NAME);
        kmessaged_dev_major = MAJOR(devno); 
    }

    if (result < 0) {
        printk(KERN_WARNING "kmessaged: can't get major %d, err: %d\n", kmessaged_dev_major, result);

        return result;
    }

    printk(KERN_DEBUG "kmessaged: instantiated with major %d and minor %d\n", kmessaged_dev_major, kmessaged_dev_minor);

    mdev = kmalloc(sizeof(struct kmessaged_message_dev_t), GFP_KERNEL);

    result = kmessaged_message_dev_init(mdev);

    if (result) {
        printk(KERN_DEBUG "kmessaged: device cannot be initialized\n");

        goto bailout;
    }

    devno = MKDEV(kmessaged_dev_major, kmessaged_dev_minor);
    cdev_init(&mdev->cdev, &kmessaged_ops);
    mdev->cdev.owner = THIS_MODULE;
    mdev->cdev.ops = &kmessaged_ops;

    result = cdev_add(&mdev->cdev, devno, 1);

    if (result) {
        printk(KERN_NOTICE "kmessaged: device couldn't be added: %d\n", result);

        goto bailout;
    }

    return 0;

bailout:
    kfree(mdev);

    return result;
}

/**
 * kmessaged_exit
 *
 * Performs cleanup and gracefully terminates the module.
 */
static void __exit kmessaged_exit(void)
{
    kmessaged_message_dev_release(mdev);
    kfree(mdev);

    printk(KERN_DEBUG "kmessaged: terminating\n");
}

module_init(kmessaged_init);
module_exit(kmessaged_exit);


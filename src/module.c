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
#include <asm/system.h>
#include <asm/uaccess.h>

#include "kmessaged.h"
#include "kmessage.h"

#define KMESSAGED_MAJOR

extern const unsigned int KMESSAGED_MAJOR_VERSION;
extern const unsigned int KMESSAGED_MINOR_VERSION;
extern const unsigned int KMESSAGED_NR_DEVS;
extern const char KMESSAGED_DV_NAME;

unsigned int kmessaged_major_version = KMESSAGED_MAJOR_VERSION;
unsigned int kmessaged_minor_version = KMESSAGED_MINOR_VERSION;
unsigned int kmessaged_nr_devs = KMESSAGED_NR_DEVS;

module_param(KMESSAGED_MAJOR_VERSION, int, S_IRUGO);
module_param(KMESSAGED_MINOR_VERSION, int, S_IRUGO);

MODULE_AUTHOR("Bugra Ekuklu, Muratcan Sahin");
MODULE_DESCRIPTION("Kernel message daemon as a character device amongst OS users.");
MODULE_LICENSE("MIT");
MODULE_VERSION("0.1");

struct kmessaged_ops kmessaged_ops = {
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
static int __init kmessaged_init()
{
    int result = 0;
    size_t i = 0;
    dev_t devno = 0;

    if (KMESSAGED_MAJOR_VERSION) {
        devno = MKDEV(KMESSAGED_MAJOR_VERSION, KMESSAGED_MINOR_VERSION);
        result = register_chrdev_region(devno, KMESSAGED_NR_DEVS, KMESSAGED_DV_NAME);
    } else {
        result = alloc_chrdev_region(&devno, KMESSAGED_MINOR_VERSION, KMESSAGED_NR_DEVS, KMESSAGED_DV_NAME);
        kmessaged_major_version = MAJOR(devno); 
    }

    if (result < 0) {
        printk(KERN_WARNING "kmessaged: can't get major %d\n", kmessaged_major_version);

        return result;
    }

    return 0;
}

/**
 * kmessaged_exit
 *
 * Performs cleanup and gracefully terminates the module.
 */
static void __exit kmessaged_exit()
{
    //  do some cleanup...
}

module_init(kmessaged_init);
module_exit(kmessaged_exit);


#include "ioctl.h"

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/module.h>
#include <asm/switch_to.h>
#include <asm/uaccess.h>

#include "reset.h"
#include "read_mode.h"
#include "message_limit.h"
#include "message_dev.h"

int kmessaged_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    int err = 0;
    int result = 0;
    struct kmessaged_message_dev_t *dev = filp->private_data;
    char __user *username = (void *)0x0;
    char *buf, *uname;
    long len = 0;
    uid_t uid;

    printk(KERN_DEBUG "kmessaged: ioctl handler called with %lu, %u, %u\n", filp, cmd, arg);

    //  Checks whether provided IOCTL command is valid, returns ENOTTY if not.
    if (_IOC_TYPE(cmd) != KMESSAGED_IOC_MAGIC) return -ENOTTY;
    if (_IOC_NR(cmd) > KMESSAGED_IOC_MAXNR) return -ENOTTY;

    if (_IOC_DIR(cmd) & _IOC_READ) {
        err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
    } else if (_IOC_DIR(cmd) & _IOC_WRITE) {
        err = !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
    }

    if (err) {
        printk(KERN_DEBUG "kmessaged: ioctl caller is not capable\n");

        return -EFAULT;
    }

    switch (cmd) {
        case KMESSAGED_IOC_S_REGUID:
            printk(KERN_DEBUG "kmessaged: ioctl S_REGUID is called\n");

            buf = kmalloc(sizeof(char) * 255, GFP_KERNEL);

            len = strncpy_from_user(buf, arg, 255);

            printk(KERN_DEBUG "kmessaged: targetstr copied from user-space: %s with len %d\n", buf, len);

            uname = kmalloc(sizeof(char) * 255, GFP_KERNEL);

            sscanf(buf, "%u:%s", &uid, uname);
            
            result = kmessaged_message_dev_add_target(dev, uname, uid);

            if (result) {
                printk(KERN_DEBUG "kmessaged: could not add target to the device: %d\n", result);

                return result;
            }

            printk(KERN_DEBUG "kmessaged: added target %u:%s to device\n", uid, uname);

            break;
        case KMESSAGED_IOC_S_RDMOD:
            result = __get_user(dev->rdmod, (int __user *)arg);

            if (result != 0) {
                return result;
            }

            break;

        case KMESSAGED_IOC_RDMOD:
            result = __put_user(dev->rdmod, (int __user *)arg);
            break;
        
        case KMESSAGED_IOC_S_MSGLMT:
            result = __get_user(dev->msglmt, (unsigned long __user *)arg);
            
            if (result != 0) {
                return result;
            }

            break;
        
        case KMESSAGED_IOC_MSGLMT:
            result = __put_user(dev->msglmt, (unsigned long __user *)arg);

            if (result != 0) {
                return result;
            }

            break;

        case KMESSAGED_IOC_RMMSG:
            //  Preallocate resources for strncpy
            username = kmalloc(sizeof(char) * KMESSAGED_MAX_USERNAME_LEN, GFP_KERNEL);

            //  FIXME: Possible kernel exploit via buffer overrun.
            if ((result = strncpy_from_user(username, (const char __user *)arg, KMESSAGED_MAX_USERNAME_LEN)) < 0) {
                printk(KERN_ERR "kmessaged: strncpy_from_user() returned %d", result);

                return -EFAULT;
            }

            //  TODO: Delete the messages.

            //  Release the preallocated resources
            kfree(username);
            break;

        /* gtest-disable-codecov */
        default:
            return -ENOTTY;
        /* gtest-enable-codecov */
    }

    return result;
}

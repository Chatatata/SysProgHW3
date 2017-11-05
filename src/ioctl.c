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
#include "query.h"

int kmessaged_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    int err = 0;
    int result = 0;
    kmessaged_read_mode_t rdmod;
    unsigned long long msglmt;
    char *username = (void *)0x0;

    //  Checks whether provided IOCTL command is valid, returns ENOTTY if not.
    if (_IOC_TYPE(cmd) != KMESSAGED_IOC_MAGIC) return -ENOTTY;
    if (_IOC_NR(cmd) > KMESSAGED_IOC_MAXNR) return -ENOTTY;

    if (_IOC_DIR(cmd) & _IOC_READ) {
        err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
    } else if (_IOC_DIR(cmd) & _IOC_WRITE) {
        err = !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
    }

    if (err) {
        return -EFAULT;
    }

    switch (cmd) {
        case KMESSAGED_IOC_RESET:
            result = kmessaged_reset();
            break;

        case KMESSAGED_IOC_S_RDMOD:
            result = __get_user(rdmod, (int __user *)arg);

            if (result != 0) {
                return result;
            }

            result = kmessaged_set_read_mode(rdmod);
            break;

        case KMESSAGED_IOC_RDMOD:
            result = kmessaged_get_current_read_mode(&rdmod);

            result = __put_user(rdmod, (int __user *)arg);
            break;
        
        case KMESSAGED_IOC_S_MSGLMT:
            result = __get_user(msglmt, (unsigned long __user *)arg);
            
            if (result != 0) {
                return result;
            }

            result = kmessaged_set_message_limit(msglmt);
            break;
        
        case KMESSAGED_IOC_MSGLMT:
            msglmt = kmessaged_get_message_limit();

            result = __put_user(msglmt, (unsigned long __user *)arg);
            break;

        case KMESSAGED_IOC_RMMSG:
            //  Preallocate resources for strncpy
            username = kmalloc(sizeof(char) * KMESSAGED_MAX_USERNAME_LEN, GFP_KERNEL);

            //  FIXME: Possible kernel exploit via buffer overrun.
            if ((result = strncpy_from_user(username, (const char __user *)arg, KMESSAGED_MAX_USERNAME_LEN)) < 0) {
                printk(KERN_ERR "kmessaged: strncpy_from_user() returned %d", result);

                return -EFAULT;
            }

            result = kmessaged_delete_messages_with_query(KMESSAGED_QUERY_NAME, username);

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

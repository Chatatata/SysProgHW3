#include "read_mode.h"

#include <linux/kernel.h>

kmessaged_read_mode_t kmessaged_current_rdmod;
static DEFINE_MUTEX(kmessaged_rdmod_lock);

int kmessaged_set_read_mode(kmessaged_read_mode_t rdmod) 
{
    //  Should check for admin privileges
    if (!capable(CAP_SYS_ADMIN)) {
        printk(KERN_DEBUG "kmessaged: read mode not set - insufficient permissions\n");

        return -EPERM;
    }

    mutex_lock(&kmessaged_rdmod_lock);
    kmessaged_current_rdmod = rdmod;
    mutex_unlock(&kmessaged_rdmod_lock);

    printk(KERN_DEBUG "kmessaged: read mote set (unsafe!): %d\n", kmessaged_current_rdmod);

    return 0;
}

int kmessaged_get_current_read_mode(kmessaged_read_mode_t *rdmodptr)
{
    if (!rdmodptr) {
        return -EINVAL;
    }

    if (!capable(CAP_SYS_ADMIN)) {
        return -EPERM;
    }

    printk(KERN_DEBUG "kmessaged: current read mode (unsafe!): %d\n", kmessaged_current_rdmod);

    mutex_lock(&kmessaged_rdmod_lock);
    *rdmodptr = kmessaged_current_rdmod;
    mutex_unlock(&kmessaged_rdmod_lock);

    return 0;
}

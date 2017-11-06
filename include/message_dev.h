#pragma once

#include "kmessaged.h"
#include "message_limit.h"
#include "read_mode.h"

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/types.h>

struct kmessaged_message_dev_target_t {
    char *name;
    uid_t uid;
};

struct kmessaged_message_dev_t {
    struct kmessage_t *unread_msgs;
    unsigned long unread_cnt;
    struct kmessage_t *read_msgs;
    unsigned long read_cnt;
    unsigned long resize_cnt;
    struct cdev cdev;
    kmessaged_read_mode_t rdmod;
    msglmt_t msglmt;

    /* Targets of the message device registered by the module driver */
    struct kmessaged_message_dev_target_t targets[255];
    ssize_t target_cnt;
    struct semaphore target_dsema;
};

KMESSAGED_EXPORT int kmessaged_message_dev_init(struct kmessaged_message_dev_t *mdev);

KMESSAGED_EXPORT int kmessaged_message_dev_add(struct kmessaged_message_dev_t *mdev, const struct kmessage_t *msg);

KMESSAGED_EXPORT int kmessaged_message_dev_readall(struct kmessaged_message_dev_t *mdev);

KMESSAGED_EXPORT int kmessaged_message_dev_expand(struct kmessaged_message_dev_t *mdev);

KMESSAGED_EXPORT int kmessaged_message_dev_add_target(struct kmessaged_message_dev_t *mdev, const char *uname, const uid_t uid);

KMESSAGED_EXPORT uid_t kmessaged_message_dev_find_target(struct kmessaged_message_dev_t *mdev, const char *uname);

KMESSAGED_EXPORT int kmessaged_message_dev_resolve_target(struct kmessaged_message_dev_t *mdev, const char *uname, const uid_t uid);

KMESSAGED_EXPORT int kmessaged_message_dev_release(struct kmessaged_message_dev_t *mdev);


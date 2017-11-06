#pragma once

#include "kmessaged.h"
#include "message_limit.h"

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/cdev.h>
#include <linux/slab.h>

struct kmessaged_message_dev_t {
    struct kmessage_t *unread_msgs;
    unsigned long unread_cnt;
    struct kmessage_t *read_msgs;
    unsigned long read_cnt;
    unsigned long resize_cnt;
    struct cdev cdev;
};

KMESSAGED_EXPORT int kmessaged_message_dev_init(struct kmessaged_message_dev_t *mdev);

KMESSAGED_EXPORT int kmessaged_message_dev_add(struct kmessaged_message_dev_t *mdev, const struct kmessage_t *msg);

KMESSAGED_EXPORT int kmessaged_message_dev_readall(struct kmessaged_message_dev_t *mdev);

KMESSAGED_EXPORT int kmessaged_message_dev_expand(struct kmessaged_message_dev_t *mdev);

KMESSAGED_EXPORT int kmessaged_message_dev_release(struct kmessaged_message_dev_t *mdev);


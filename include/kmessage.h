#pragma once

#include <linux/uidgid.h>

#include "kmessaged.h"

struct kmessage_t {
    char *msg;
    unsigned long len;
    uid_t uid;
};

/**
 * kmessaged_msg_init
 *
 * Initializes a new `kmessage_t` object on given struct pointer in kernel address space.
 *
 * @param msgstruct Pointer to the object being initialized.
 * @param len Length of the message.
 * @param uid The `uid` value of the user.
 */
KMESSAGED_EXPORT int kmessaged_msg_init(const kmessage_t *msgstruct, const unsigned long len, const uid_t uid);

/**
 * kmessaged_msg_create
 *
 * Creates a new message with given C string.
 *
 * @param msgdata The *null-terminated* C string to be used in message.
 * @param uid The `uid` value of the user.
 */
KMESSAGED_EXPORT int kmessaged_msg_create(const char *msgdata, const uid_t uid);

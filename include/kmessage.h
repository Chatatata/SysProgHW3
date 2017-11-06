#pragma once

#include <linux/uidgid.h>

#include "kmessaged.h"

struct kmessage_t {
    char *msg;
    unsigned long len;
    uid_t uid;
    const char *recipient;
};

/**
 * kmessaged_msg_create
 *
 * Creates a new message with given preallocated C string.
 *
 * @param msgstruct Pointer to the object being initialized.
 * @param msgdata The *null-terminated* C string to be used in message.
 * @param uid The `uid` value of the user.
 * @param recipient The recipient of the message sent.
 *
 * @return 0 if successful, EINVAL if invalid parameter provided, ENOMEM if no memory has left on device.
 */
KMESSAGED_EXPORT int kmessaged_msg_create(struct kmessage_t *msgstruct, const char *msgdata, const uid_t uid, const char *recipient);

/**
 * kmessaged_msg_release
 *
 * Releases the given message struct.
 *
 * @param msgstruct Pointer to the object going to be released.
 * 
 * @return This function always returns 0.
 */
KMESSAGED_EXPORT int kmessaged_msg_release(struct kmessage_t *msgstruct);


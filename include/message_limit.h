#pragma once

#include "kmessaged.h"

#include <linux/fs.h>

typedef int msglmt_t;

/**
 * kmessaged_set_message_limit
 *
 * Sets the message limit of the message box to specified value.
 *
 * @param msglmt The new message limit.
 *
 * @returns Returns 0 if successful. Otherwise, returns -EPERM
 * if permission is denied.
 */
KMESSAGED_EXPORT int kmessaged_set_message_limit(msglmt_t msglmt);

/**
 * kmessaged_get_message_limit
 *
 * Returns the current message limit of the message box.
 *
 * @returns Current message limit of the box.
 */
KMESSAGED_EXPORT msglmt_t kmessaged_get_message_limit(void);


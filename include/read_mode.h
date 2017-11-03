#pragma once

#include "kmessaged.h"

/**
 * kmessaged_read_mode_t
 *
 * Indicates the read mode of the `kmessaged`. If `EXCLUDE_READ`
 * is set, `kmessaged` device will not write unread messages to the
 * stream. Otherwise, if `INCLUDE_READ` is set, device will also
 * write unread messages to the stream.
 *
 * @see kmessaged_set_read_mode
 * @see kmessaged_get_current_read_mode
 */
typedef enum kmessaged_read_mode_t {
    EXCLUDE_READ = 0,
    INCLUDE_READ = 1,
} kmessaged_read_mode_t;

/**
 * kmessaged_set_read_mode
 *
 * Sets the read mode to the given value.
 *
 * @returns Returns 0 if successful, -EFAULT in case of an error.
 */
KMESSAGED_EXPORT int kmessaged_set_read_mode(kmessaged_read_mode_t rdmod);

/**
 * kmessaged_read_mode_t
 *
 * Returns the current read mode `kmessaged` is working on.
 *
 * @param rdmodptr Pointer to the `kmessaged_read_mode_t` struct.
 *
 * @returns Returns 0 if successful, -EFAULT in case of an error, -EINVAL if parameter is NULL.
 */
KMESSAGED_EXPORT int kmessaged_get_current_read_mode(kmessaged_read_mode_t *rdmodptr);


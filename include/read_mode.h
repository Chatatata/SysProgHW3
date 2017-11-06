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


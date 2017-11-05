#pragma once

#include "kmessaged.h"

/**
 * kmessaged_reset
 *
 * Resets the kernel message daemon. Requires admin privileges.
 *
 * @returns Returns 0 on success, otherwise -EFAULT.
 */
KMESSAGED_EXPORT int kmessaged_reset(void);

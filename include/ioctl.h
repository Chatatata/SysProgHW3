#pragma once

#include "kmessaged.h"

#include <linux/fs.h>

KMESSAGED_EXPORT int kmessaged_ioctl(struct file *filp, unsigned int cmd, void *arg);

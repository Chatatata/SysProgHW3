#pragma once

#include "kmessaged.h"

#include <linux/fs.h>

KMESSAGED_EXPORT extern int kmessaged_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);

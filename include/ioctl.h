#pragma once

#include "kmessaged.h"

KMESSAGED_EXPORT int kmessaged_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, void *arg);

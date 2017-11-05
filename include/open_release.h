#pragma once

#include "kmessaged.h"

#include <linux/fs.h>

KMESSAGED_EXPORT int kmessaged_open(struct inode *inode, struct file *filep);

KMESSAGED_EXPORT int kmessaged_release(struct inode *inode, struct file *filp);

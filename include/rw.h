#pragma once

#include "kmessaged.h"

KMESSAGED_EXPORT ssize_t kmessaged_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);

KMESSAGED_EXPORT ssize_t kmessaged_write(struct file *filep, const char __user *buf, size_t count, loff_t *f_pos);

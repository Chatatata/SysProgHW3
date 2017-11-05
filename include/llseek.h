#pragma once

#include "kmessaged.h"

#include <linux/types.h>
#include <linux/fs.h>

KMESSAGED_EXPORT loff_t kmessaged_llseek(struct file *filp, loff_t off, int whence);


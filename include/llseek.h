#pragma once

#include "kmessaged.h"

KMESSAGED_EXPORT loff_t kmessaged_llseek(struct file *filp, loff_t off, int whence);


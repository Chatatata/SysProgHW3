#!/bin/sh

clear
make clean
make
nm kmessaged.ko
sudo rmmod kmessaged 1>> /dev/null 2>&1
sudo insmod kmessaged.ko

#!/bin/sh

clear
sudo make clean
sudo make
nm kmessaged.ko
sudo rmmod kmessaged 1>> /dev/null 2>&1
sudo unlink /dev/kmessaged0
sudo insmod kmessaged.ko
sudo lsmod
dmesg | grep kmessaged

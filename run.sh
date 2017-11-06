#!/bin/sh

DEVICE_NODE_NAME=kmessaged0

echo "Cleaning build directory."
sudo make clean 1>> /dev/null 2>&1

echo "Building kernel module kmessaged."
sudo make 1>> /dev/null 2>&1

echo "Removing kernel module if exists."
sudo rmmod kmessaged 1>> /dev/null 2>&1 
if [ $? -ne 0 ]; then
    echo "kmessaged was not inserted."
fi

sudo unlink /dev/kmessaged0 1>> /dev/null 2>&1 

if [ $? -ne 0 ]; then
    echo "Device is not attached."
fi

echo "Inserting module kmessaged.ko."
sudo insmod kmessaged.ko

echo "Creating device node \"$DEVICE_NODE_NAME\"."
sudo mknod /dev/$DEVICE_NODE_NAME c `dmesg | tail -n 1 | cut -d ' ' -f 6` 0

echo "Applying chmod of device node."
sudo chmod 777 /dev/$DEVICE_NODE_NAME

echo "Cleaning helper build directory."
make -C helper -s clean 1>> /dev/null 2>&1

echo "Removing module header file from helper build directory."
rm ./helper/include/kmessaged.h 1>> /dev/null 2>&1

echo "Copying module header file to helper build directory."
cp ./include/kmessaged.h ./helper/include/kmessaged.h

echo "Compiling helper."
make -C helper -s 1>> /dev/null 2>&1

echo "Starting helper."
./helper/helper 1>> /dev/null 2>&1


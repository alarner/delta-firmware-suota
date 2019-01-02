#!/bin/bash

# Check if virtualbox is installed
hash VBoxManage
if [ ! $? -eq 0 ]; then
    echo "VBoxManage command doesn't exist. Have you installed VirtualBox?"
    exit 1
fi

# Check if the climbalytics-win image exists
if [ $(echo -n "$(VBoxManage list vms | grep climbalytics-win)" | wc -m) -eq "0" ]; then
	echo "\"climbalytics-win\" virtual machine doesn't exist. You can download the image here..."
	exit 2
fi

# Start the VM
if [ $(echo -n "$(VBoxManage list runningvms | grep climbalytics-win)" | wc -m) -eq "0" ]; then
	echo "Adding shared folder..."
	ABSOLUTE_PATH="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
	VBoxManage sharedfolder remove climbalytics-win --name climbalytics
	VBoxManage sharedfolder add climbalytics-win --name climbalytics --hostpath $ABSOLUTE_PATH/bin --automount
	echo "Starting \"climbalytics-win\" virtual machine..."
	VBoxManage startvm "climbalytics-win" --type headless
	echo "\"climbalytics-win\" booting up. Waiting 40s..."
	sleep 40s
fi

#!/bin/bash

if ! command -v xset &> /dev/null; then
	echo "Please run \`sudo apt install x11-xserver-utils\`!"
	exit 1
fi

if ! timeout 1s xset q &>/dev/null; then
	echo "You do not have VcXsrv running!"
	exit 1
fi

echo "Add these commands to your ~/.basrc later"
set -o verbose
export DISPLAY=$(awk '/nameserver / {print $2; exit}' /etc/resolv.conf 2>/dev/null):0
export LIBGL_ALWAYS_INDIRECT=0

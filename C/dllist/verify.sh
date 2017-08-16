#!/bin/bash

if [ `cat dllist.h | md5sum | cut -f1 -d' '` = "698504046a19722101cdc53943cc22e4" ]; then
	exit 0
else
	echo "dllist.h has been modified. Please re-download it from assignments"
	exit 1
fi

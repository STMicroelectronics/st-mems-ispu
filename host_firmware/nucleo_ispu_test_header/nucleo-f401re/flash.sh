#!/bin/bash

script_dir=$(dirname $0)

programmer=STM32_Programmer_CLI
if ! command -v $programmer &> /dev/null
then
	programmer=STM32_Programmer_CLI.exe
	if ! command -v $programmer &> /dev/null
	then
		echo "Cannot find STM32_Programmer_CLI command"
		exit 1
	fi
fi

if [ $# -gt 1 ]
then
	echo "Usage: flash.sh [probe_index]"
	exit 2
fi

num_probes=$($programmer --list | grep "ST-Link Probe" | wc -l)

probe=0
if [ $# -eq 1 ]
then
	probe=$1
elif [ $num_probes -gt 1 ]
then
	echo "More than one ST-Link probe detected, please specify the index of the probe to use."
	echo "The available probes can be listed with: $programmer --list."
	echo "Usage: flash.sh <probe_index>"
	exit 3
fi

$programmer -c port=SWD index=$probe -w $script_dir/build/nucleo_ispu_test_header.bin 0x8000000 -s 0x8000000


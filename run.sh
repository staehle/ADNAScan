#!/bin/bash
if [ "$1" == "" ] || [ "$2" == "" ] || [ "$3" == "" ]; then
	echo "Usage: $0 <num_processes> <fastq1> <fastq2> (opt_name)"
	echo
	exit 1
fi

CUSTOM=""
if [ "$4" != "" ]; then
	$CUSTOM=$4
fi

mkdir -p results
rm -rf ./results/*

./bin/adna-init $1 $2 $3 $custom
mpirun -np $1 ./bin/adna-gompi

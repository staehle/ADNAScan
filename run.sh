#!/bin/bash
mkdir -p results
rm -rf ./results/*
if [ "$1" == "" ] || [ "$2" == "" ] || [ "$3" == "" ]; then
	echo "Usage: $0 <num_processes> <fastq1> <fastq2>"
	echo
	exit 1
fi

mpirun -np $1 $2 $3

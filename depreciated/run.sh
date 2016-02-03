#!/bin/bash
if [ "$1" == "" ] || [ "$2" == "" ] || [ "$3" == "" ]; then
	echo "Usage: $0 <num_processes> <fastq1> <fastq2>"
	echo
	exit 1
fi
mkdir -p results
rm -rf ./results/*
mpirun -np $1 ./bin/adna $2 $3

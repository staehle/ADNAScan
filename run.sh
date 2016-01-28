#!/bin/bash
if [ "$1" == "" ] || [ "$2" == "" ] || [ "$3" == "" ]; then
	echo "Usage: $0 <num_processes> <fastq1> <fastq2> (opt_name)"
	echo
	exit 1
fi

mkdir -p results
rm -rf ./results/*

if [ "$4" == "" ]; then
	./bin/adna-init $1 $2 $3
else
	./bin/adna-init $1 $2 $3 $4
fi

if [ $? == 0 ]; then
	mpirun -np $1 ./bin/adna-gompi
else
	echo "adna run script stopped due to initialization error"
fi



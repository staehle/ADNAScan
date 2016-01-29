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
	nohup mpirun -np $1 ./bin/adna-gompi &
	sleep 2
	echo "adna-gompi has been daemonized. use ./adna-check to see progress."
	echo "When reported as complete, run ./adna-finish to cleanup the job."
else
	echo "ERROR: adna run script stopped due to initialization error"
fi


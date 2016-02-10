#!/bin/bash
LD_LIBRARY_PATH=/usr/local/gcc-5.3.0/lib64/:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH

if [ "$1" == "" ] || [ "$2" == "" ] || [ "$3" == "" ]; then
	echo "Usage: $0 <num_processes> <fastq1> <fastq2> (opt_name)"
	echo
	exit 1
fi

mkdir -p results
mkdir -p results/curjob
rm -rf ./results/curjob/*
mkdir -p results/curjob/ind

if [ "$4" == "" ]; then
	./bin/adna-init $1 $2 $3
else
	./bin/adna-init $1 $2 $3 $4
fi

if [ $? == 0 ]; then
	nohup /usr/local/gcc-5.3.0/openmpi/bin/mpirun -np $1 ./bin/adna-gompi &
	sleep 2
	echo "adna-gompi has been daemonized. use ./adna-check to see progress or manage job tasks."
	./bin/adna-check 
else
	echo "ERROR: adna run script stopped due to initialization error"
fi






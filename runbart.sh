#!/bin/bash
if [ "$1" == "" ] || [ "$2" == "" ] || [ "$3" == "" ]; then
	echo "Usage: $0 <num_processes> <fastq1> <fastq2>"
	echo
	exit 1
fi
LD_LIBRARY_PATH=/usr/local/gcc-5.3.0/lib64/:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH
mkdir -p results
#rm -rf ./results/*
/usr/local/gcc-5.3.0/openmpi/bin/mpirun -np $1 ./bin/adna $2 $3

DATE=`date +%Y%m%d_%H_%M`
id=$2
id=${id::${#id}-5}
#id=${`sed 's/.\{5\}$//' <<< "$2"`}
id+="$DATE"
r1P=$id+"_1Pass.fastq"
r2p=$id+"_2Pass.fastq"
r1F=$id+"_1Fail.fastq"
r2F=$id+"_2Fail.fastq"
sRP=$id+"_singlePass.fastq"
sRF=$id+"_singleFail.fastq"


for i in `ls | grep -E "*read1Pass*"` ; do
	cat $i >> $r1P
done

echo $r1P

for i in `ls | grep -E "*read2Pass*"` ; do
	cat $i >> $r2P
done

for i in `ls | grep -E "*read1Fail*"` ; do
	cat $i >> $r1F
done

for i in `ls | grep -E "*read2Fail*"` ; do
	cat $i >> $r2F
done

for i in `ls | grep -E "*singleReadPass*"` ; do
	cat $i >> $sRP
done

for i in `ls | grep -E "*singleReadFail*"` ; do
	cat $i >> $sRF
done

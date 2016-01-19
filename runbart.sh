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

cd ./results

DATE=`date +%Y%m%d_%H_%M`
DATE=${DATE::${#DATE}-1}
id=$2
id=${id::${#id}-5}
id=${id##*/}
#id=${`sed 's/.\{5\}$//' <<< "$2"`}
id+="$DATE"
r1P=$id"_r1Pass.fastq"
r2p=$id"_r2Pass.fastq"
r1F=$id"_r1Fail.fastq"
r2F=$id"_r2Fail.fastq"
sRP=$id"_singleReadPass.fastq"
sRF=$id"_singleReadFail.fastq"

touch $r1P
touch $r2P
touch $r1F
touch $r2F
touch $sRP
touch $sRF


for i in `ls | grep -E "*read1Pass*"` ; do
	cat $i >> $r1P
	rm $i
done

echo $r1P

for i in `ls | grep -E "*read2Pass*"` ; do
	cat $i >> $r2P
	rm $i
done

for i in `ls | grep -E "*read1Fail*"` ; do
	cat $i >> $r1F
	rm $i
done

for i in `ls | grep -E "*read2Fail*"` ; do
	cat $i >> $r2F
	rm $i
done

for i in `ls | grep -E "*singleReadPass*"` ; do
	cat $i >> $sRP
	rm $i
done

for i in `ls | grep -E "*singleReadFail*"` ; do
	cat $i >> $sRF
	rm $i
done

cd ..


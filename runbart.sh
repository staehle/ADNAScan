#!/bin/bash
if [ "$1" == "" ] || [ "$2" == "" ] || [ "$3" == "" ]; then
	echo "Usage: $0 <num_processes> <fastq1> <fastq2>"
	echo
	exit 1
fi
LD_LIBRARY_PATH=/usr/local/gcc-5.3.0/lib64/:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH
mkdir -p results

/usr/local/gcc-5.3.0/openmpi/bin/mpirun -np $1 ./bin/adna $2 $3

cd ./results

DATE=`date +%Y%m%d_%H_%M`
id=$2
id=${id::${#id}-5}
id=${id##*/}
#id=${`sed 's/.\{5\}$//' <<< "$2"`}
id+="$DATE"

mkdir -p $id

r1P="r1Pass.fastq"
r2P="r2Pass.fastq"
r1F="r1Fail.fastq"
r2F="r2Fail.fastq"
sRP="singleReadPass.fastq"
sRF="singleReadFail.fastq"

aRem="removedAdapters.out"
bReads="goodBadReadsCount.out"
mCount="mergeCount.out"
tRem="tRemoveCount.out"

touch ./$id/$r1P
touch ./$id/$r2P
touch ./$id/$r1F
touch ./$id/$r2F
touch ./$id/$sRP
touch ./$id/$sRF

touch ./$id/$aRem
touch ./$id/$bReads
touch ./$id/$mCount
touch ./$id/$tRem


for i in `ls | grep "read1Pass_"` ; do
	cat $i >> ./$id/$r1P
	rm $i
done

for i in `ls | grep "read2Pass_"` ; do
	cat $i >> ./$id/$r2P
	rm $i
done

for i in `ls | grep "read1Fail_"` ; do
	cat $i >> ./$id/$r1F
	rm $i
done

for i in `ls | grep "read2Fail_"` ; do
	cat $i >> ./$id/$r2F
	rm $i
done


for i in `ls | grep "singleReadPass_"` ; do
	cat $i >> ./$id/$sRP
	rm $i
done

for i in `ls | grep "singleReadFail_"` ; do
	cat $i >> ./$id/$sRF
	rm $i
done

for i in `ls | grep "aRem_"` ; do
	cat $i >> ./$id/$aRem
	rm $i
done

for i in `ls | grep "badReads_"` ; do
	cat $i >> ./$id/$bReads
	rm $i
done

for i in `ls | grep "merges_"` ; do
	cat $i >> ./$id/$mCount
	rm $i
done

for i in `ls | grep "tRem_"` ; do
	cat $i >> ./$id/$tRem
	rm $i
done


cd ..


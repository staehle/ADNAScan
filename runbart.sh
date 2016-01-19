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
r1P=$id"_r1Pass.fastq"
r2P=$id"_r2Pass.fastq"
r1F=$id"_r1Fail.fastq"
r2F=$id"_r2Fail.fastq"
sRP=$id"_singleReadPass.fastq"
sRF=$id"_singleReadFail.fastq"

aRem=$id"_removedAdapters.out"
bReads=$id"_goodBadReadsCount.out"
mCount=$id"_mergeCount.out"
tRem=$id"_tRemoveCount.out"

touch $r1P
touch $r2P
touch $r1F
touch $r2F
touch $sRP
touch $sRF

touch $aRem
touch $bReads
touch $mCount
touch $tRem


for i in `ls | grep -E "*read1Pass*"` ; do
	cat $i >> $r1P
	rm $i
done

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


#for i in `ls | grep -E "*singleReadPass*"` ; do
#	cat $i >> $sRP
#	rm $i
#done

#for i in `ls | grep -E "*singleReadFail*"` ; do
#	cat $i >> $sRF
#	rm $i
#done

for i in `ls | grep -E "*aRem*"` ; do
	cat $i >> $aRem
	rm $i
done

for i in `ls | grep -E "*badReads*"` ; do
	cat $i >> $bReads
	rm $i
done

for i in `ls | grep -E "*merges*"` ; do
	cat $i >> $mCount
	rm $i
done

for i in `ls | grep -E "*tRem*"` ; do
	cat $i >> $tRem
	rm $i
done


cd ..


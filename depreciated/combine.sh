#!/bin/bash

p=`pwd`
cd $1

r1P="$2_pass_1.fastq"
r2P="$2_pass_2.fastq"
r1F="$2_fail_1.fastq"
r2F="$2_fail_2.fastq"
sRP="$2_sPass.fastq"
sRF="$2_sFail.fastq"

aRem="removedAdapters.temp"
bReads="goodBadReadsCount.temp"
mCount="mergeCount.temp"
tRem="tRemoveCount.temp"

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


for i in `ls | grep "read1Pass_"` ; do
	cat $i >> $r1P
	rm $i
done

for i in `ls | grep "read2Pass_"` ; do
	cat $i >> $r2P
	rm $i
done

for i in `ls | grep "read1Fail_"` ; do
	cat $i >> $r1F
	rm $i
done

for i in `ls | grep "read2Fail_"` ; do
	cat $i >> $r2F
	rm $i
done


for i in `ls | grep "singleReadPass_"` ; do
	cat $i >> $sRP
	rm $i
done

for i in `ls | grep "singleReadFail_"` ; do
	cat $i >> $sRF
	rm $i
done

for i in `ls | grep "aRem_"` ; do
	cat $i >> $aRem
	rm $i
done

for i in `ls | grep "badReads_"` ; do
	cat $i >> $bReads
	rm $i
done

for i in `ls | grep "merges_"` ; do
	cat $i >> $mCount
	rm $i
done

for i in `ls | grep "tRem_"` ; do
	cat $i >> $tRem
	rm $i
done


cd $p


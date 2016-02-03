#!/bin/bash

if [ $# -ne 1 ] ; then
  echo Must have argument of path to results folder to fix
  exit
fi

curD=`pwd`
idDir=$1

cd $idDir

ctr=0
while read line ; do
  if (( $ctr % 4 == 0 )) ; then
    line="@"$line
  fi
  echo $line >> r1Pass_UD.fastq
  ctr=$((ctr+1))
done <r1Pass.fastq


ctr=0
while read line2 ; do
  if (( $ctr % 4 == 0 )) ; then
    line2="@"$line2
  fi
  echo $line2 >> r2Pass_UD.fastq
  ctr=$((ctr+1))
done <r2Pass.fastq


ctr=0
while read line3 ; do
  if (( $ctr % 4 == 0 )) ; then
    line3="@"$line3
  fi
  echo $line3 >> singleReadPass_UD.fastq
  ctr=$((ctr+1))
done <singleReadPass.fastq

cd $curD

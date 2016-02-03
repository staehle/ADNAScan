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
while read line ; do
  if (( $ctr % 4 == 0 )) ; then
    line="@"$line
  fi
  echo $line >> r2Pass_UD.fastq
  ctr=$((ctr+1))
done <r2Pass.fastq


ctr=0
while read line ; do
  if (( $ctr % 4 == 0 )) ; then
    line="@"$line
  fi
  echo $line >> singleReadPass_UD.fastq
  ctr=$((ctr+1))
done <singleReadPass.fastq

cd $curD

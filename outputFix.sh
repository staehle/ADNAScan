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
  echo $line >> temp_UD.fastq
done <mergeCount.temp

cd $curD

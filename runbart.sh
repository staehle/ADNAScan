#!/bin/bash
LD_LIBRARY_PATH=/usr/local/gcc-5.3.0/lib64/:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH

alias mpirun="/usr/local/gcc-5.3.0/openmpi/bin/mpirun"

./run.sh $@



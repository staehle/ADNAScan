![adna logo][logo]

**adna** is the *A*synchronous-process *DNA* fastq checker and trimmer


## about ##

The *adna* project was started in 2015 as a senior project at the University of North Dakota, created by Jake Staehle, Brandon Vovk, and Steinar Bakke.

*adna* is an *A*wesome *DNA* utility. It utilizes the OpenMPI library system to provide asynchronous processing of DNA fastq data sets. It supports fasta adapter files for trimming. It merges two reads. It doesn't do much else right now. 

Work in progress. 


## building ##

#### you have: ####

- GCC 4.8 or above
- OpenMPI
- cmake
- libncurses

`# apt install g++-4.8 mpi-default-bin mpi-default-dev libncurses5-dev`

*Stuck with an older version of GCC? Try a side-by-side installation: [Gist: GCC Switch][gccsw]*

#### you want: ####

`$ cmake ./`

`$ make `

`$ run.sh <num_processes> <read1.fastq> <read2.fastq> [<optional_job_name>]`

`$ cat ./results/<job_name.time>/results.txt`


[logo]: https://github.com/staehle/adna/raw/master/logo-250.png
[gccsw]: https://gist.github.com/staehle/9d7221edf4f03cb38e5a28b2596253f8


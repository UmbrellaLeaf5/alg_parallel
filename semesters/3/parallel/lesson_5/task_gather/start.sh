#!/bin/bash

#PBS -l walltime=00:01:00,nodes=4:ppn=2
#PBS -N ul_lesson5
#PBS -q batch

cd $PBS_O_WORKDIR
mpirun --hostfile $PBS_NODEFILE -np 7 ./a.out

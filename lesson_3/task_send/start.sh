#!/bin/bash

#PBS -l walltime=00:01:00,nodes=1:ppn=2
#PBS -N ul_lesson3
#PBS -q batch

cd $PBS_O_WORKDIR
mpirun --hostfile $PBS_NODEFILE -np 2 ./a.out
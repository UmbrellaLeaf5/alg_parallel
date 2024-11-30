#!/bin/bash

#PBS -l walltime=00:01:00,nodes=1:ppn=4
#PBS -N ul_lesson_hybrid
#PBS -q batch

cd $PBS_O_WORKDIR
export OMP_NUM_THREADS=$PBS_NUM_PPN
mpirun  --hostfile $PBS_NODEFILE -pernode -x \
OMP_NUM_THREADS -np $PBS_NUM_NODES ./a.out 1000

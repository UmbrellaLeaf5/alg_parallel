#!/bin/bash

#PBS -l walltime=00:01:00,nodes=3:ppn=2
#PBS -N falt202420_lesson_6
#PBS -q batch

cd $PBS_O_WORKDIR
mpirun --hostfile $PBS_NODEFILE -np 2 ./a.out
mpirun --hostfile $PBS_NODEFILE -np 4 ./a.out
mpirun --hostfile $PBS_NODEFILE -np 6 ./a.out

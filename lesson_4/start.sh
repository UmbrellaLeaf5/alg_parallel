#!/bin/bash

#PBS -l walltime=00:01:00,nodes=4:ppn=2
#PBS -N ul_lesson_4
#PBS -q batch

cd $PBS_O_WORKDIR
mpirun --hostfile $PBS_NODEFILE -np 1 ./a.out
mpirun --hostfile $PBS_NODEFILE -np 2 ./a.out
mpirun --hostfile $PBS_NODEFILE -np 3 ./a.out
mpirun --hostfile $PBS_NODEFILE -np 4 ./a.out
mpirun --hostfile $PBS_NODEFILE -np 5 ./a.out
mpirun --hostfile $PBS_NODEFILE -np 6 ./a.out
mpirun --hostfile $PBS_NODEFILE -np 7 ./a.out

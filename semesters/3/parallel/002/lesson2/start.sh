#!/bin/bash

#PBS -l walltime=00:01:00,nodes=3:ppn=2
#PBS -N falt202420_lesson2
#PBS -q batch

cd $PBS_O_WORKDIR
mpirun --hostfile $PBS_NODEFILE -np 6 ./a.out

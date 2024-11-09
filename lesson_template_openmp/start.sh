#!/bin/bash

#PBS -l walltime=00:01:00,nodes=1:ppn=4
#PBS -N ul_lesson_template
#PBS -q batch

cd $PBS_O_WORKDIR
export OMP_NUM_THREADS=$PBS_NUM_PPN
./a.out

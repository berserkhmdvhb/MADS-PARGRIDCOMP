# PARGRIDCOMP
This repository is dedicated to the project of the course parallel and grid computing.
Image filtering algorithms are implemented in both serial and parallel versions.
We write a report on it which we will attach soon.


In the following, the commands used in the HPC cluster is written:

ssh -p 8022 ekhaveh@access-iris.uni.lu

si -N 1 --exclusive -t 01:00:00

git clone [https://github.com/berserkhmdvhb/PARGRIDCOMP](https://github.com/berserkhmdvhb/PARGRIDCOMP)

cd PARGRIDCOMP/filter-parallel

module purge

module load mpi/OpenMPI/4.0.5-GCC-10.2.0

//module load swenv/default-env/latest

module load tools/VTune/2020_update3

module load toolchain/intel/2020b


icc -o filterpar filter.c helpers.c -lm -qopenmp


./filterpar -g images/large.bmp output/out.bmp

aps --collection-mode=all -r report ./filterpar -g images/large.bmp output/out.bmp

---
For APS:


ssh -p 8022 ekhaveh@access-iris.uni.lu

si -N 1 --exclusive -t 01:00:00

git clone [https://github.com/berserkhmdvhb/PARGRIDCOMP](https://github.com/berserkhmdvhb/PARGRIDCOMP)

cd PARGRIDCOMP/filter-parallel-2

module purge

module load mpi/OpenMPI/4.0.5-GCC-10.2.0

module load tools/VTune/2020_update3

module load toolchain/intel/2020b

icc -qopenmp filter.c helpers.c -lm 

aps --collection-mode=all -r report ./a.out

aps-report -g report

scp -r hvaheb@iris-150://home/hamed/Documents/Luxembourg/Courses/Semester3/PARGRIDCOMP/filter-parallel-2/report/ /home/hamed/Documents/Luxembourg/Courses/Semester3/PARGRIDCOMP/filter-parallel-2/report/


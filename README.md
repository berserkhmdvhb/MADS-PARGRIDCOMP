# PARGRIDCOMP

This repository is dedicated to the course project for **Parallel and Grid Computing**. It implements image filtering algorithms in both **serial** and **parallel** versions using OpenMP and MPI.

The project includes performance testing and profiling on a High-Performance Computing (HPC) cluster.

---

## Features

- Serial image filtering
- Parallel image filtering using OpenMP
- Advanced parallel filtering (APS) with MPI and VTune profiling
- Benchmarks and performance evaluations

---

## Directory Structure

```
.
├── filter               # Serial filtering implementation
├── filter-parallel      # OpenMP-based parallel filtering
├── filter-parallel-2    # APS version with VTune profiling
├── README.md
├── Report.pdf           # Project report
└── .gitattributes
```

---

## Installation and Compilation

### Requirements

- GCC with OpenMP support
- MPI (e.g., OpenMPI)
- Intel VTune (for APS version)

### Build Commands

**Serial:**
```bash
cd filter
gcc -o filter filter.c helpers.c -lm
```

**OpenMP Parallel:**
```bash
cd filter-parallel
gcc -o filteropmp filter.c helpers.c -lm -fopenmp
```

**APS with MPI and VTune:**
```bash
cd filter-parallel-2
module load mpi/OpenMPI/4.0.5-GCC-10.2.0
module load tools/VTune/2020_update3
mpicc -o filteraps filter.c helpers.c -lm
```

---

## Running the Project

### Locally

**OpenMP example:**
```bash
./filteropmp -g images/large.bmp output/output.bmp
```

### On the UL HPC Cluster

```bash
ssh -p 8022 ekhaveh@access-iris.uni.lu

# Request an interactive job
si -N 1 --exclusive -t 01:00:00

# Clone the repository
git clone https://github.com/berserkhmdvhb/PARGRIDCOMP.git
cd PARGRIDCOMP/filter-parallel

# Load modules
module purge
module load mpi/OpenMPI/4.0.5-GCC-10.2.0

# Compile and run
gcc -o filteropmp filter.c helpers.c -lm -fopenmp
./filteropmp -g images/large.bmp output/output.bmp
```

#### For APS Version

```bash
ssh -p 8022 ekhaveh@access-iris.uni.lu
si -N 1 --exclusive -t 01:00:00

git clone https://github.com/berserkhmdvhb/PARGRIDCOMP.git
cd PARGRIDCOMP/filter-parallel-2

module purge
module load mpi/OpenMPI/4.0.5-GCC-10.2.0
module load tools/VTune/2020_update3

mpicc -o filteraps filter.c helpers.c -lm
# Profiling example (optional, using VTune)
vtune -collect hotspots ./filteraps -g images/large.bmp output/output.bmp
```

---

## Report

The project report detailing design choices, experiments, and results is available as `Report.pdf`.

---

## License

This project is provided for academic use as part of the Parallel and Grid Computing course. Licensing terms can be adjusted based on future publication or collaboration needs.

---

## Authors

- **Ehsan Khaveh** – [GitHub](https://github.com/berserkhmdvhb)

Feel free to reach out for questions or collaborations.

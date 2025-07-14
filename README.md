# PARGRIDCOMP

**Parallel and Grid Computing Project**
📷 Parallel Image Filtering in C using OpenMP and MPI
📊 Performance profiling with Intel VTune on UL's HPC Cluster (Iris)

---

## 🧩 Overview

This project implements basic image filtering algorithms (grayscale, blur, edge detection, reflect) in both **serial** and **parallel** forms using:

* **OpenMP** for shared-memory parallelism
* **MPI** for distributed-memory parallelism
* **Intel VTune Profiler** for performance analysis

The project was developed and tested on the **UL HPC Iris cluster** and includes performance benchmarking across different thread counts and scheduling strategies.

---

## 🚀 Features

* ✅ Serial image filters implemented in C
* 🧵 OpenMP-based parallel filtering with static, dynamic, and guided scheduling
* 💠 MPI-based parallelism and analysis using Intel VTune APS
* 📈 Runtime benchmarking on small and large images
* 📄 Detailed project report with profiling insights

---

## 📁 Directory Structure

```
.
├── filter/              # Serial implementation
├── filter-parallel/     # OpenMP-based parallel version
├── filter-parallel-2/   # MPI + VTune profiled version
├── images/              # Test input images (not included)
├── output/              # Output image results
├── Report.pdf           # Full technical report
└── README.md
```

---

## ⚙️ Requirements

* GCC with OpenMP support
* OpenMPI
* Intel VTune Profiler (for APS version)
* UL HPC Cluster access (optional)

---

## 🛠️ Build Instructions

### Serial Version

```bash
cd filter
gcc -o filter filter.c helpers.c -lm
```

### OpenMP Parallel Version

```bash
cd filter-parallel
gcc -o filteropmp filter.c helpers.c -lm -fopenmp
```

### MPI + VTune APS Version

```bash
cd filter-parallel-2
module load mpi/OpenMPI/4.0.5-GCC-10.2.0
module load tools/VTune/2020_update3
mpicc -o filteraps filter.c helpers.c -lm
```

---

## ▶️ Running the Code

### Example (Local Execution with OpenMP)

```bash
./filteropmp -g images/large.bmp output/output.bmp
```

* `-g`: Grayscale (`-b` = blur, `-e` = edge, `-r` = reflect)

---

## 💻 Running on the UL HPC Cluster

```bash
ssh -p 8022 <your-username>@access-iris.uni.lu
si -N 1 --exclusive -t 01:00:00
```

### OpenMP Example

```bash
git clone https://github.com/berserkhmdvhb/PARGRIDCOMP.git
cd PARGRIDCOMP/filter-parallel

module purge
module load mpi/OpenMPI/4.0.5-GCC-10.2.0
gcc -o filteropmp filter.c helpers.c -lm -fopenmp

./filteropmp -g images/large.bmp output/output.bmp
```

### APS + MPI Version with VTune

```bash
cd PARGRIDCOMP/filter-parallel-2

module purge
module load mpi/OpenMPI/4.0.5-GCC-10.2.0
module load tools/VTune/2020_update3

mpicc -o filteraps filter.c helpers.c -lm

# Optional profiling with Intel VTune
vtune -collect hotspots ./filteraps -g images/large.bmp output/output.bmp
```

---

## 📈 Performance Summary

* OpenMP runtime benchmarks were tested with 2, 4, 8, and 16 threads.
* **Static scheduling** performed best overall.
* For low-complexity filters and small images, the **serial version outperformed parallel versions**.
* On large images (53.7 MB), using **2 threads with static scheduling** gave the best runtime.
* Intel VTune profiling revealed that the `__kmp_fork_barrier` function became a performance bottleneck as thread count increased.

📄 See `Report.pdf` for full analysis, including runtime tables, thread utilization histograms, and bottleneck discussion.

---

## 🧐 Key Learnings

* Thread overhead can dominate performance when computational complexity is low.
* Static scheduling reduces overhead in tightly scoped loops.
* Profiling tools like Intel VTune are essential for uncovering non-obvious performance issues.
* Image size and algorithm complexity directly affect the benefits of parallelism.

---



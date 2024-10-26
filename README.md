
# Mandelbrot Set GPU Optimization with OpenACC

This project implements a Mandelbrot set generator optimized for parallel computation on both CPU and GPU. Using OpenACC on the GPU and OpenMP on the CPU, we measure and compare performance across different OpenACC optimizations.

## Table of Contents
- [Project Overview](#project-overview)
- [Setup and Requirements](#setup-and-requirements)
- [Compilation and Execution](#compilation-and-execution)
- [Performance Results](#performance-results)
- [OpenACC Optimization Experiments](#openacc-optimization-experiments)
- [Discussion](#discussion)
- [Conclusions](#conclusions)

## Project Overview

The Mandelbrot program visualizes the Mandelbrot set by computing color values for each pixel based on complex number calculations. We used OpenACC to parallelize the workload for GPU execution and OpenMP for CPU, enabling a detailed performance comparison between both architectures. 

The experiments ran on `node08` in our lab's HPC cluster, yielding the following metrics:
- Calculation time
- Memory usage

## Setup and Requirements

### Prerequisites
- **C Compiler** (e.g., GCC)
- **OpenACC and OpenMP** support:
  - Ensure your GPU supports OpenACC.
  - Install `OpenMP` for CPU parallelization.

### Files
- **mandelbrot_gpu.c** - OpenACC-optimized GPU version of the Mandelbrot program.
- **mandelbrot_cpu.c** - OpenMP-optimized CPU version for performance comparison.

## Compilation and Execution

### GPU Execution with OpenACC
1. Compile the Mandelbrot program for GPU using OpenACC:
   ```bash
   pgcc -acc -o mandelbrot_gpu mandelbrot_gpu.c
   ```
2. Run the program on the GPU:
   ```bash
   ./mandelbrot_gpu
   ```

### CPU Execution with OpenMP
1. Compile the Mandelbrot program for CPU using OpenMP:
   ```bash
   gcc -fopenmp -o mandelbrot_cpu mandelbrot_cpu.c
   ```
2. Run the program on the CPU:
   ```bash
   ./mandelbrot_cpu
   ```

## Performance Results

The following table summarizes the CPU vs. GPU performance for the Mandelbrot program. Calculation times and memory usage were collected from each run:

| Test       | CPU Calculation Time | CPU Memory Used | GPU Calculation Time | GPU Memory Used |
|------------|-----------------------|-----------------|-----------------------|-----------------|
| Mandelbrot | 12.234 seconds        | 8,294,400 bytes | 6.315 seconds        | 8,294,400 bytes |

## OpenACC Optimization Experiments

We experimented with several OpenACC clauses to optimize GPU performance, yielding the following results:

| OpenACC Clauses            | Calculation Time       | Memory Used         |
|----------------------------|------------------------|---------------------|
| gang                        | 6.315716 seconds      | 8,294,400 bytes     |
| gang + worker              | 6.329285 seconds      | 8,294,400 bytes     |
| gang + worker + vector     | 6.329026 seconds      | 8,294,400 bytes     |
| gang + vector              | 6.305781 seconds      | 8,294,400 bytes     |

## Discussion

The experiments reveal that optimizing GPU execution with OpenACC clauses significantly impacts performance. The `gang` clause alone serves as an effective baseline, distributing work across GPU cores efficiently. Adding the `worker` clause increased calculation time slightly, likely due to added complexity in parallel thread management, indicating the Mandelbrot program may not benefit from finer-grained parallelism at the worker level.

The `gang + vector` configuration produced the best results, with a calculation time of 6.305781 seconds. This demonstrates that simple parallelism structures may be more suitable for the Mandelbrot program’s independent calculations per pixel.

## Conclusions

OpenACC provides a powerful framework for optimizing parallel computations on GPUs. In this Mandelbrot program, the `gang + vector` configuration yielded the best performance, achieving a balance between calculation time and memory use. This experiment underscores the need for workload-specific tuning to maximize GPU efficiency.

By comparing both OpenMP on CPU and OpenACC on GPU, we demonstrate that OpenACC accelerates computation significantly, making it a suitable choice for highly parallelizable applications like Mandelbrot set generation.


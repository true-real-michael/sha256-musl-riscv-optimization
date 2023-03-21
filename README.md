# sha256-musl-riscv-optimization
This repository contains source code related to optimization of musl-libc SHA-256 function for 64 bit RISC-V architecture.
It also contains source code for benchmarking the proposed modifications.

Paper: [Оптимизация алгоритма SHA-256 библиотеки musl под архитектуру RISC-V](https://se.math.spbu.ru/thesis/onreview/Kiselev_Mihail_Bachelor_Report_2023_text_36f6ce02021428c1.pdf)

# Building and Running
Download musl from [official repository](https://musl.libc.org/).

Build the modified version of musl using:

```./build.py --musldir MUSLDIR [--prefix PREFIX] [--unrolling] [--parallelization] [-O {1,2,3,s}]```

Its arguements are:
* `MUSLDIR` is a valid directory containing musl source code.
* `PREFIX` is a directory where musl will be installed, defaults to `./build`.
* `--unrolling` option enables unrolling of the compression loop of the SHA-256 algorithm.
* `--parallelization` option enables parallelization of the expansion loop of the SHA-256 algorithm.
* `-O` specifies the type of optimization used by `gcc` when converting SHA-256 implementation from C to Assembly.

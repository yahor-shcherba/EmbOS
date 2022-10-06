# EmbOS

__EmbOS__ - multithreading microkernel for the x86 architecture. This OS always running in kernel mode without user space, also work in single addressing space. The operating system implements threads and mutexes.

Development was carried out in a unix-like environment, including the gcc tool

## Installing dependecies
Development was carried out using only gnu c compiler and qemu emulator

```bash
# install nasm and qemu on debian/ubuntu
sudo apt install gcc
sudo apt install qemu
```

## Source organization
```
  ./include and ./libc
    This directory contains header and source files, a like in the standard c library
  ./sys
    this directory contains the kernel code.
```

## Building and running
```bash
https://github.com/yahor-shcherba/EmbOS
make        # compiling
make qemu   # run os in qemu
```

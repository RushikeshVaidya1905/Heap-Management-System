# 🧠 Heap Memory Management System

## Description

This project implements a **custom heap memory manager** in C, simulating the behavior of `malloc()` and `free()` functions. It demonstrates how dynamic memory allocation and deallocation can be handled manually using a simulated heap, with added logic to efficiently manage memory reuse and fragmentation.

The system supports:

- Memory allocation using `allocate(size_t size)`
- Memory deallocation using `free(void* ptr)`
- **First-fit** allocation strategy
- **Block splitting** for efficient memory use
- **Block coalescing** to merge adjacent free blocks and reduce fragmentation

---

## Features

- Simulates a 1 MB heap using a static array.
- Allocates memory blocks with metadata tracking.
- Reuses freed memory efficiently.
- Merges adjacent free blocks automatically.
- Demonstrates low-level memory management without using built-in `malloc()` or `free()`.

---


# Multithreaded Linux Application Using POSIX Threads and Mutexes


## Team Details

**Section:** 10
**Team:** 8

### Team Members

| Roll Number | Name            |
| ----------- | --------------- |
| 2520090170  | S Svara Haasini |
| 2520090227  | B Sudharshini   |
| 2520090190  | Sumanjali       |

### Supervisor

**Mr. M. Raghupathi**

---

## Abstract

Modern multi-core systems rely on concurrent execution for efficiency and responsiveness, but when multiple threads within a process access shared resources at the same time, it can cause race conditions and inconsistent results. This is a key challenge in Operating Systems, since threads share the same memory space and, without proper synchronization, can corrupt shared data.

This project addresses this by developing a multithreaded Linux application using POSIX threads (Pthreads) for concurrent execution and mutex locks to synchronize access to critical sections, ensuring correctness and thread safety.

---

## Objectives

1. To understand and apply the concept of multithreading in a Linux environment using the POSIX Threads (Pthreads) library.
2. To design and implement a C program that creates multiple threads to perform concurrent tasks efficiently.
3. To identify critical sections in shared data access and use mutex locks to prevent race conditions and ensure data consistency.
4. To evaluate thread synchronization, thread joining, and safe resource sharing, and to demonstrate correct, thread-safe program execution on Linux.

---

## Technologies Used

* Linux / Ubuntu
* C / C++
* POSIX Threads (Pthreads)
* GCC Compiler
* VS Code
* GDB

---

## Operating Systems Concepts / APIs

* Process and Thread Management
* `pthread_create()`
* `pthread_join()`
* `pthread_mutex_t`
* `pthread_mutex_lock()`
* `pthread_mutex_unlock()`
* Critical Sections
* Race Condition Handling

---

## Proposed Methodology

The project is developed in C on a Linux (Ubuntu) platform using the GCC compiler. The main program creates multiple worker threads using `pthread_create()`. The worker threads perform tasks involving a shared resource.

A mutex is used to protect the critical section. `pthread_mutex_lock()` is called before accessing the shared resource and `pthread_mutex_unlock()` is called after the critical section. The main thread uses `pthread_join()` to wait for all worker threads to complete.

The program will also be tested with and without mutex protection to demonstrate the effect of synchronization on correctness.

---

## Project Structure

```text
├── src/
│   └── Source code
│
├── docs/
│   └── Project documentation and diagrams
│
├── data/
│   └── Data/source references
│
├── results/
│   └── Program outputs and comparison results
│
├── reports/
│   └── Project reports
│
└── README.md
```

---

## Setup Instructions

### Requirements

* Ubuntu/Linux environment
* GCC compiler
* POSIX Threads library
* VS Code (optional)
* GDB (optional)

### Compilation

The project will be compiled using GCC with POSIX Threads support:

```bash
gcc src/main.c -o multithreaded_app -pthread
```

---

## Execution

Run the compiled application using:

```bash
./multithreaded_app
```

The program output will be used to verify thread execution, synchronization, and shared-resource consistency.

---

## Expected Outcome

A fully functional multithreaded Linux application will be developed that correctly creates, manages, and synchronizes multiple POSIX threads.

The project will demonstrate safe concurrent access to shared resources, with mutex locks preventing race conditions and ensuring accurate and consistent output.

A comparison between execution with and without mutex synchronization will demonstrate the effect of synchronization on program correctness.

---

## Individual Contributions

| Team Member     | Responsibility                                                          |
| --------------- | ----------------------------------------------------------------------- |
| S Svara Haasini | Overall program structure, `pthread_create()`, documentation and report |
| B Sudharshini   | Mutex locking mechanism and race-condition testing                      |
| Sumanjali       | `pthread_join()`, output consistency, debugging and Linux testing       |

---

## Current Phase Status

**Current Status:** Project repository setup completed.

* [x] Repository created
* [x] Required directory structure created
* [x] README created
* [ ] Source code implementation
* [ ] Thread synchronization implementation
* [ ] Testing with and without mutex
* [ ] Results and comparison
* [ ] Final documentation
* [ ] Final project evaluation

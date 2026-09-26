# Multithreaded Linux Application Using POSIX Threads and Mutexes

## Team Details

**Section:** 10  
**Team:** 8

### Team Members

| Roll Number | Name |
|---|---|
| 2520090170 | S Svara Haasini |
| 2520090227 | B Sudharshini |
| 2520090190 | Sumanjali |

### Supervisor

**Mr. M. Raghupathi**

---

## Project Overview

This project demonstrates multithreading, race conditions, critical sections, and synchronization in Linux using POSIX threads and mutex locks.

Multiple worker threads access and update a shared counter.

The program can be executed:

1. Without mutex synchronization
2. With mutex synchronization
3. In both modes for comparison

Without synchronization, multiple threads may access the shared resource at the same time and cause lost updates.

With mutex synchronization, only one thread is allowed to enter the critical section at a time, producing the correct result.

---

## Main Operating System Concepts

The project demonstrates the following Operating Systems concepts:

- Multithreading
- POSIX Threads
- Shared Resources
- Critical Sections
- Race Conditions
- Mutual Exclusion
- Mutex Locks
- Thread Creation
- Thread Joining
- Process Creation using `fork()`
- Program Execution using `exec()`
- Custom Linux Shell

---

## POSIX Thread APIs Used

The multithreaded application uses:

- `pthread_create()`
- `pthread_join()`
- `pthread_mutex_init()`
- `pthread_mutex_lock()`
- `pthread_mutex_unlock()`
- `pthread_mutex_destroy()`

---

## Custom Linux Shell

The project also includes a custom Linux shell implemented in C.

The shell provides a terminal-style prompt:

```text
my_shell$

The following commands can be executed through the shell:
ls
pwd
date
whoami
echo
clear
cd
project
exit

The special command:
project

launches the multithreaded mutex application.
The custom shell uses concepts such as:
- fork()
- execvp()
- execl()
- waitpid()
- Process creation
- Child process execution
- Linux command execution
Project Execution Flow
Ubuntu Terminal
      |
      | ./my_shell
      v
Custom Shell
      |
      | my_shell$ project
      v
Multithreaded Linux Application
      |
      +--> 1. Run WITHOUT Mutex
      |
      +--> 2. Run WITH Mutex
      |
      +--> 3. Run BOTH

Multithreaded Application
The program creates:
8 worker threads

Each thread performs:
100000 counter increments

Therefore, the expected final result is:
8 × 100000 = 800000

Mode 1 — Without Mutex
When the program runs without synchronization, multiple threads may read and update the shared counter at the same time.
This creates a race condition.
Example:
Expected Result : 800000
Actual Result   : 189308
Status          : RACE CONDITION DETECTED
Lost Updates    : 610692

The exact incorrect value may change between executions because thread scheduling is unpredictable.
Mode 2 — With Mutex
When mutex synchronization is enabled, the shared counter is protected using:
pthread_mutex_lock(&mutex);

and:
pthread_mutex_unlock(&mutex);

Only one thread can update the counter inside the critical section at a time.
Example:
Expected Result : 800000
Actual Result   : 800000
Status          : CORRECT / THREAD SAFE

Mode 3 — Comparison
Option 3 executes both versions one after another.
WITHOUT MUTEX
      |
      v
Race Condition / Lost Updates

WITH MUTEX
      |
      v
Correct Result / Thread Safe

This makes the effect of synchronization easy to observe.
Project Structure
Multithreaded_Project/
│
├── src/
│   ├── shell.c
│   └── project.c
│
├── .github/
│   └── workflows/
│       └── run-project.yml
│
├── docs/
├── data/
├── reports/
├── results/
│
├── Makefile
└── README.md

Important Source Files
src/shell.c
Contains the custom Linux shell.
src/project.c
Contains the POSIX threads and mutex demonstration.
Makefile
Compiles both programs.
.github/workflows/run-project.yml
Allows the project to be compiled and demonstrated using GitHub Actions.
Requirements
The project requires:
- Linux / Ubuntu
- GCC Compiler
- POSIX Threads library
- Make
Compilation
Move to the project directory:
cd ~/Multithreaded_Project

Compile the custom shell and the multithreaded application:
make

This creates:
my_shell
project

These are compiled executable files.
Running the Project Locally
Start the custom shell:
./my_shell

On the WSL development environment used for this project, the custom shell opens in a new terminal window.
The shell displays:
========================================
          OSSP CUSTOM SHELL
========================================

my_shell$

To launch the multithreaded application:
my_shell$ project

The application displays:
=====================================
 MULTITHREADED LINUX APPLICATION
=====================================

1. Run WITHOUT Mutex
2. Run WITH Mutex
3. Run BOTH

Enter choice:

Enter 1, 2, or 3 depending on the required demonstration.
Running Through GitHub Actions
The project can also be compiled and demonstrated directly through GitHub Actions.
Open:
GitHub Repository
      ↓
Actions
      ↓
OSSP Project Demo
      ↓
Run workflow

The workflow provides three execution modes:
1 - Run WITHOUT Mutex
2 - Run WITH Mutex
3 - Run BOTH and Compare

The GitHub workflow:
1. Checks out the repository
2. Compiles the custom shell
3. Compiles the multithreaded application
4. Starts the custom shell
5. Executes Linux shell commands
6. Launches the project using the project command
7. Runs the selected mutex mode
8. Displays the result in the workflow log
Expected Results
Without Mutex
The final counter value is normally less than:
800000

because race conditions cause lost updates.
With Mutex
The final counter value should be:
800000

because the critical section is protected.
Conclusion
This project demonstrates why synchronization is necessary in multithreaded systems.
Without synchronization, concurrent access to shared data can cause race conditions and incorrect results.
Using a POSIX mutex protects the critical section and ensures that the shared resource is updated safely.
The project also integrates a custom Linux shell, process creation, command execution, POSIX threads, and synchronization concepts into a single Linux-based Operating Systems application.
Current Project Status
Core project implementation completed successfully.
- [x] Repository created
- [x] Project directory structure created
- [x] POSIX thread implementation completed
- [x] Multiple worker threads implemented
- [x] Shared counter implemented
- [x] Race condition demonstration completed
- [x] Mutex synchronization implemented
- [x] Thread joining implemented
- [x] Without-mutex mode implemented
- [x] With-mutex mode implemented
- [x] Comparison mode implemented
- [x] Custom Linux shell implemented
- [x] Basic Linux commands supported
- [x] project command integrated into custom shell
- [x] Local execution tested successfully
- [x] GitHub Actions workflow implemented
- [x] GitHub Actions execution tested successfully

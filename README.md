# Thread Pool

## About the Project
🧵 **Thread Pool** is a lightweight concurrent job scheduler implemented in C.  
It works like a basic **worker pool**, spinning up `n` threads that share a **circular job queue** — dispatching tasks in parallel without the overhead of spawning a new thread per job.

This project was built for learning purposes, focusing on **thread synchronisation**, **mutex/condition variable patterns**, and **object-oriented design in C**.

---

## 🧩 Core Functions

- `init_tpool(int n)` — creates a new thread pool with `n` worker threads.
- `deploy(t_thread *this, void (*f)(void *), void *arg)` — submits a job (function + argument) to the queue.
- `wait(t_thread *this, int n)` — blocks until `n` jobs have completed.
- `destroy(t_thread *this)` — signals all threads to quit, joins them, and frees all memory.

---

## How to Use

Clone the repository:
```bash
git clone git@github.com:PedroLouzada/Thread_Pool.git
```

Include the header in your project:
```c
#include "thread.h"
```

Example usage:
```c
t_thread *pool = init_tpool(4);

pool->deploy(pool, my_task, &my_arg);
pool->deploy(pool, my_task, &other_arg);

pool->wait(pool, 2);   // wait for both jobs to finish
pool->destroy(pool);   // clean up
```

---

## ⚙️ Implementation Notes

- Workers loop indefinitely, sleeping on a **condition variable** until a job is available.
- The job queue is a **circular buffer** of fixed size (10 slots).
- If the queue is full, `deploy` drops the job silently — callers should pace submissions.
- `destroy` broadcasts to all threads to wake and exit, then joins each one cleanly.
- The pool is exposed through a **vtable-style interface** (`t_thread *`), hiding internal state behind the base struct.

---

## Contact
[GitHub](https://github.com/PedroLouzada)

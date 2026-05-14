*This project has been created as part of the 42 curriculum by dde-paul*

# Codexion

## Description

Codexion is a concurrency simulation inspired by the classic **Dining Philosophers problem**. In this project, a group of coders sits around a table, each needing **two dongles** (left and right) to compile quantum code. After compiling, coders debug and refactor their code before attempting another compilation.  

The simulation stops under two conditions:  
1. A coder **burns out** (fails to start compiling within `time_to_burnout` milliseconds).  
2. All coders have completed the **required number of compilations**.

The challenge is to manage **shared resources (dongles)** among multiple threads safely, preventing deadlocks, race conditions, and starvation while respecting cooldowns and deadlines.

---

## Instructions

### Compilation

```bash
make        # Compile the program
make re     # Recompile from scratch
make clean  # Remove object files
make fclean # Remove object files and the executable
````

### Usage

```bash
./codexion <number_of_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <number_of_compiles_required> <dongle_cooldown> <scheduler>
```

**Parameters:**

| Parameter                       | Type    | Description                                                    |
| ------------------------------- | ------- | -------------------------------------------------------------- |
| `<number_of_coders>`            | int ≥ 1 | Number of coders sitting at the table                          |
| `<time_to_burnout>`             | ms      | Maximum time a coder can wait before burnout                   |
| `<time_to_compile>`             | ms      | Time spent compiling code                                      |
| `<time_to_debug>`               | ms      | Time spent debugging                                           |
| `<time_to_refactor>`            | ms      | Time spent refactoring                                         |
| `<number_of_compiles_required>` | int     | Number of compilations required per coder                      |
| `<dongle_cooldown>`             | ms      | Minimum cooldown time after releasing a dongle                 |
| `<scheduler>`                   | string  | `fifo` (first-in-first-out) or `edf` (earliest deadline first) |

**Example:**

```bash
./codexion 4 1500 200 200 100 2 100 fifo
```

---

## Blocking Cases Handled

The project addresses several concurrency challenges:

1. **Race conditions**

   * Occur when multiple threads access shared resources simultaneously.
   * All shared resource accesses are protected with **mutexes**.

2. **Deadlocks (Coffman conditions)**

   * **Mutual exclusion**: Only one coder can hold a dongle at a time.
   * **Hold-and-wait**: Coders hold one dongle while waiting for the other.
   * **No preemption**: Dongles cannot be forcibly taken.
   * **Circular wait**: Prevented by always acquiring dongles in **increasing memory order**.

3. **Starvation prevention**

   * FIFO queues or EDF heaps ensure coders are served fairly.
   * Retry with backoff (`usleep(1000–50000)`) gives other coders a chance to progress.

4. **Cooldown handling**

   * After release, dongles cannot be re-acquired until the cooldown expires.
   * Implemented with timestamps and `pthread_cond_timedwait`.

5. **Burnout detection**

   * A monitor thread checks coders every 5 ms.
   * Simulation stops if a coder fails to compile before their deadline.

6. **Log serialization**

   * All logs are protected with a global **print_mutex** to prevent interleaving.

---

## Thread Synchronization Mechanisms

1. **Mutexes (`pthread_mutex_t`)**

   * **Dongle mutex**: Protects dongle state (`available`, `held_by`, `timestamp`, queue/heap).
   * **Print mutex**: Ensures atomic printing of logs.

2. **Condition Variables (`pthread_cond_t`)**

   * Each dongle has a condition variable to block coders when unavailable.
   * `pthread_cond_broadcast` wakes all waiting coders when a dongle is released.

3. **FIFO queues & EDF heaps**

   * **FIFO**: Coders served in arrival order.
   * **EDF**: Coders with earliest deadline served first.

4. **Monitor thread**

   * Runs every 5 ms, checks for burnout, and sets `data->stop = 1`.
   * Wakes all waiting coders to exit safely.

5. **Race condition prevention**

   * Dongle state, logging, and queue/heap modifications are protected by mutexes.
   * Coders and monitor communicate safely through condition variables and shared flags.

---

## Deadlock Illustration: Resource Allocation Graph (RAG)

In this simulation, coders are threads and dongles are shared resources.
A deadlock occurs if a **circular wait** happens: each coder is holding one dongle while waiting for another.

### ASCII Representation

```
        ┌─────────────────────────────────┐
        │                                 │
        ▼                                 │
   ┌─────────┐                       ┌─────────┐
   │ Dongle 1│                       │ Dongle 2│
   │ (left)  │                       │ (right) │
   └────┬────┘                       └────┬────┘
        │                                 │
        │ held by C1                      │ held by C2
        │                                 │
        ▼                                 ▼
   ┌─────────┐                       ┌─────────┐
   │ Coder 1 │                       │ Coder 2 │
   └────┬────┘                       └────┬────┘
        │                                 │
        │ waits for Dongle 2              │ waits for Dongle 1
        │                                 │
        └───────────────┬─────────────────┘
                        │
                        ▼
                  (DEADLOCK!)
```

**Description:**

* Rectangles `[Coder X]` – threads.
* Brackets `[Dongle X]` – resources.
* `▲` arrow – resource **held by** a coder.
* `◀` arrow – coder **waiting for** the resource.

A **cycle in the graph** indicates a deadlock situation.
In my solution, **acquiring dongles in memory address order breaks this cycle**, preventing deadlocks.

---

## Resources

* **The dining Philosophers in C: threads, race conditions and deadlocks #codewithme**
https://www.youtube.com/watch?v=zOpzGHwJ3MU&t=7138s
* **POSIX Threads (pthreads)** – documentation for mutexes, condition variables, and threads.
   https://man7.org/linux/man-pages/man7/pthreads.7.html

   https://www.ibm.com/docs/en/aix/7.1.0?topic=programming-using-mutexes

   https://man7.org/linux/man-pages/man3/pthread_cond_broadcast.3p.html

   https://man7.org/linux//man-pages/man3/pthread_cond_init.3.html
* **AI usage**: AI was consulted for **README structuring**

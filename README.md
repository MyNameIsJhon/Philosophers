*This project has been created as part of the 42 curriculum by jriga.*

# Philosophers

## Description

This project is a C implementation of the classic **Dining Philosophers** concurrency problem, originally formulated by Edsger Dijkstra.

**The scenario:** A number of philosophers sit around a circular table. Each philosopher alternates between three states: **thinking**, **eating**, and **sleeping**. A fork is placed between each pair of adjacent philosophers. To eat, a philosopher must pick up both the fork to their left and the fork to their right. The challenge is to coordinate access to these shared forks so that:

- No philosopher starves (every philosopher gets to eat before their `time_to_die` expires).
- No deadlock occurs (the simulation does not freeze indefinitely).
- No data race occurs (shared resources are accessed safely from concurrent threads).

Each philosopher runs as a **POSIX thread** (`pthread`), and each fork is protected by a **mutex** (`pthread_mutex_t`). A dedicated monitor thread continuously checks whether any philosopher has exceeded their `time_to_die` without eating, and stops the simulation if so.

### Key technical choices

| Mechanism | Purpose |
|---|---|
| One thread per philosopher | True concurrency, each philosopher acts independently |
| One mutex per fork | Prevents two philosophers from grabbing the same fork simultaneously |
| Per-philosopher mutexes (`last_eat_mtx`, `is_dead_mtx`) | Safe read/write of shared per-philosopher state |
| Even-philosopher stagger | Even-indexed philosophers delay their first action by `time_to_eat` to break symmetry and avoid deadlock |
| Odd-count think-time | When the number of philosophers is odd, a computed think delay is injected to reduce starvation |
| Dedicated monitor | A single supervisor loop polls each philosopher's `last_eat_ms` and triggers simulation end |

---

## Instructions

### Requirements

- A C compiler (`cc` / `gcc` / `clang`)
- POSIX threads library (standard on Linux and macOS)
- GNU Make

### Compilation

```bash
# Standard build
make

# Debug build (AddressSanitizer + UndefinedBehaviorSanitizer)
make debug

# Clean object files
make clean

# Full clean (objects + binary)
make fclean

# Rebuild from scratch
make re
```

### Usage

```
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
```

| Argument | Description |
|---|---|
| `number_of_philosophers` | Total number of philosophers (and forks) |
| `time_to_die` | Time in milliseconds before a philosopher dies if they haven't started eating |
| `time_to_eat` | Time in milliseconds a philosopher spends eating |
| `time_to_sleep` | Time in milliseconds a philosopher spends sleeping |
| `number_of_times_each_philosopher_must_eat` | *(optional)* Simulation ends when every philosopher has eaten this many times |

All time values are in **milliseconds**.

### Examples

```bash
# 5 philosophers, die at 800ms, eat for 200ms, sleep for 200ms
./philo 5 800 200 200

# 4 philosophers, must each eat at least 7 times
./philo 4 410 200 200 7

# Edge case: 1 philosopher (will always die, only one fork available)
./philo 1 800 200 200
```

### Output format

```
<timestamp_ms> <philosopher_id> <action>
```

Actions: `is thinking` | `has taken a fork` | `is eating` | `is sleeping` | `died`

```
0       1 is thinking
0       2 is thinking
200     1 has taken a fork
200     1 has taken a fork
200     1 is eating
400     1 is sleeping
...
```

---

## Visualizer

A simulation log visualizer is available (work in progress) at:
[https://github.com/MyNameIsJhon/philosophers-visualizer](https://github.com/MyNameIsJhon/philosophers-visualizer)

It allows you to paste the output of a `./philo` run and visualize the timeline of each philosopher's actions.

---

## Resources

All references used were the official man pages and documentation for the functions required by the project:

- [`pthread_create(3)`](https://man7.org/linux/man-pages/man3/pthread_create.3.html)
- [`pthread_join(3)`](https://man7.org/linux/man-pages/man3/pthread_join.3.html)
- [`pthread_mutex_init(3)`](https://man7.org/linux/man-pages/man3/pthread_mutex_init.3p.html)
- [`pthread_mutex_lock(3)` / `pthread_mutex_unlock(3)`](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html)
- [`pthread_mutex_destroy(3)`](https://man7.org/linux/man-pages/man3/pthread_mutex_destroy.3p.html)
- [`gettimeofday(2)`](https://man7.org/linux/man-pages/man2/gettimeofday.2.html)
- [`usleep(3)`](https://man7.org/linux/man-pages/man3/usleep.3.html)
- [`Dining philosophers problem — Wikipedia`](https://en.wikipedia.org/wiki/Dining_philosophers_problem)

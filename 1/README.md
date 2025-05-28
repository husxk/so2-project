# Dining Philosophers Problem – Project Documentation

## Problem Description

The dining philosophers problem is a classic synchronization problem introduced by
Edsger Dijkstra. Five philosophers sit around a circular table, each with a fork
between them (5 forks total). They alternate between thinking and eating. To eat, a
philosopher needs both the fork to their left and the fork to their right.

The challenge is to coordinate access to the shared resources (forks) in a way that
avoids:

- Deadlock – all philosophers are waiting indefinitely for forks.
- Starvation – some philosophers may never get a chance to eat.
- Race conditions – incorrect behavior due to simultaneous access to shared resources.

In this project, each philosopher is implemented as an independent thread.

---

## How to Run

1. Clone the repository:

   ```bash
   git clone https://github.com/husxk/so2-project.git
   cd so2-project
   ```

2. Build the project:

   ```bash
   ./scripts/build.sh
   ```

3. Run the program with the number of philosophers as an argument:

   ```bash
   ./build/main <number_of_philosophers>
   ```

   Example with 5 philosophers:

   ```bash
   ./build/main 5
   ```

---

## Threads and Their Roles

| Thread        | Role                          |
|---------------|-------------------------------|
| `Philosopher` | Represents one philosopher    |

Each philosopher runs in a separate thread and operates independently.

---

## Critical Sections and Solutions

**Critical section:** The act of picking up and putting down forks (accessing shared
resources).

**Solutions applied:**

- **Mutexes** – used to protect access to forks and ensure mutual exclusion.
- **Fork ordering** – philosophers always pick up one specific fork first (the 
  lower-numbered fork first), which prevents circular wait and thus deadlocks.
- **Non-blocking lock attempts** – forks are acquired using `std::unique_lock` with the
  `std::try_to_lock` tag. If a fork is already locked, the attempt fails immediately
  without blocking the thread. This ensures philosophers never block while trying to
  pick up forks, allowing them to continue thinking, or retrying. This approach
  helps avoid deadlocks by preventing threads from waiting indefinitely on mutexes.


**Solutions applied:**

- **Mutexes** – used to protect access to forks and ensure mutual exclusion.
- **Fork ordering** – philosophers always pick up one specific fork first (the
  lower-numbered fork first), which prevents circular wait and thus deadlocks.


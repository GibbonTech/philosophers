# Philosophers

## Overview
A C implementation of the classic Dining Philosophers problem, developed as part of the 42 School curriculum. This project demonstrates thread synchronization, deadlock prevention, and resource management in a multi-threaded environment.

![Dining Philosophers Visualization](https://upload.wikimedia.org/wikipedia/commons/7/7b/An_illustration_of_the_dining_philosophers_problem.png)

## The Problem
The Dining Philosophers problem is a classic synchronization challenge in computer science:
- N philosophers sit at a round table with N forks
- Each philosopher needs two forks to eat (one from each side)
- Philosophers can only eat, sleep, or think
- If a philosopher doesn't eat within a specified time, they die
- The simulation must prevent deadlocks (where philosophers can't proceed) and starvation (where a philosopher can't access resources)

## Features
- Clean thread synchronization using mutexes
- Precise timing of philosopher actions
- Death detection
- Meal counting for optional termination
- Resource management without deadlocks
- Performance optimized for minimal CPU usage

## Implementation Details
- Each philosopher is represented by a thread
- Each fork is protected by a mutex
- Proper thread safety with mutex locks for shared resources
- Accurate timestamps for philosopher state changes
- Staggered thread creation to minimize race conditions

## Project Structure
```
philosophers/
├── Makefile                     # Project build configuration
├── philosophers.c               # Main program entry point
├── includes/
│   └── philosophers.h           # Header file with struct definitions and prototypes
└── srcs/
    ├── philosopher_actions.c    # Core philosopher behaviors
    ├── philosopher_routine.c    # Main philosopher thread routine
    ├── init.c                   # Environment initialization
    ├── thread_start.c           # Thread creation and management
    ├── validation.c             # Argument validation
    ├── cleanup.c                # Resource cleanup
    ├── time_utils.c             # Time-related functions
    └── string_conversion.c      # String/number conversion utilities
```

## File Structure
The project code is organized into logically named files:

| File | Description |
|------|-------------|
| `philosophers.c` | Main program entry point |
| `philosopher_actions.c` | Core actions for philosophers (eating, thinking, etc.) |
| `philosopher_routine.c` | Main thread routine for each philosopher |
| `init.c` | Environment and philosopher initialization |
| `thread_start.c` | Thread creation and management |
| `validation.c` | Command-line argument validation |
| `cleanup.c` | Resource cleanup and memory management |
| `time_utils.c` | Time-related utility functions |
| `string_conversion.c` | String manipulation and conversion utilities |

## How to Compile and Run

### Compilation
```bash
make
```

### Running the Program
```bash
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
```

### Parameters
- `number_of_philosophers`: Number of philosophers at the table
- `time_to_die`: Time in milliseconds until a philosopher dies from starvation
- `time_to_eat`: Time in milliseconds it takes to eat
- `time_to_sleep`: Time in milliseconds a philosopher sleeps
- `number_of_times_each_philosopher_must_eat` (optional): Program stops when all philosophers have eaten this many times

### Examples
```bash
# 4 philosophers, 800ms to die, 200ms to eat, 200ms to sleep
./philo 4 800 200 200

# 5 philosophers, 800ms to die, 200ms to eat, 200ms to sleep, must eat 7 times each
./philo 5 800 200 200 7
```

### Example Output
When running the program with 4 philosophers, you'll see output like this:
```
5 1 has taken a fork
5 1 has taken a fork
5 1 is eating
205 1 is sleeping
205 2 has taken a fork
205 2 has taken a fork
205 2 is eating
405 1 is thinking
405 2 is sleeping
405 3 has taken a fork
405 3 has taken a fork
405 3 is eating
```

Each line follows the format: `<timestamp_in_ms> <philosopher_number> <action>`

## Testing and Edge Cases

The program is designed to handle various edge cases:

### Single Philosopher
With only one philosopher, they can't eat because they need two forks but only have access to one. The program correctly handles this case by having the philosopher take one fork and then die when the `time_to_die` is reached.

```bash
./philo 1 800 200 200
```

### Even vs. Odd Number of Philosophers
The program efficiently handles both even and odd numbers of philosophers by implementing a strategy that prevents deadlocks in both cases.

### Meal Counting
When the optional `number_of_times_each_philosopher_must_eat` argument is provided, the program correctly terminates once all philosophers have eaten the specified number of times.

### Error Handling
The program validates all input arguments and provides appropriate error messages for invalid inputs:
- Non-numeric values
- Negative values
- Too few or too many arguments
- Values that would cause integer overflow

## Key Challenges Addressed
- **Race Conditions**: Proper mutex usage prevents data corruption
- **Deadlocks**: Careful resource acquisition order prevents deadlocks
- **Starvation**: Philosophers are scheduled fairly to prevent starvation
- **Performance**: Efficient sleeping mechanisms minimize CPU usage
- **Proper Cleanup**: All resources are properly cleaned up on program termination

## Skills Demonstrated
- Multi-threaded programming
- Mutex synchronization
- Deadlock prevention
- Resource management
- Real-time monitoring
- System-level programming in C

## Conclusion
This project demonstrates proficiency in concurrent programming and system-level design in C. The implementation shows a deep understanding of thread synchronization, resource management, and deadlock prevention, which are critical skills for developing robust, multi-threaded applications.

The code is organized with clean, intuitive file naming and structured with maintainability in mind. It handles edge cases gracefully and provides clear feedback on invalid inputs.

## License
This project was developed as part of the 42 School curriculum.

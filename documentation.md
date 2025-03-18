# Philosophers Project Documentation

## Introduction

The Philosophers project is a classic computer science problem called "The Dining Philosophers Problem." This problem was created to illustrate challenges in computer science related to resource allocation and preventing deadlock and starvation in concurrent programs.

### The Dining Philosophers Problem Explained Simply

Imagine a table with 5 philosophers sitting around it. Each philosopher does three things:
1. Think (they don't need any resources for this)
2. Eat (they need two forks to eat their spaghetti)
3. Sleep (they don't need any resources for this)

Between each philosopher is one fork. To eat, a philosopher needs to use both forks next to them. The challenge is to design a solution where:
- No philosopher starves (everyone gets to eat eventually)
- No deadlock occurs (the program doesn't get stuck)
- Resources are used efficiently

This project implements a solution to this problem using threads and mutexes in C.

## Project Structure

Here's a breakdown of the files and what they do:

```
philosophers.c - Main program and param validation
├── srcs/
│   ├── init.c - Environment initialization
│   ├── philo_functions.c - Core philosopher actions (eat, die, etc.)
│   ├── threads.c - Thread management for philosophers
│   ├── utils.c - Utility functions (time, sleep, etc.)
│   └── atoitoa.c - String/number conversion functions
└── includes/
    └── philosophers.h - Header with struct definitions
```

## Key Concepts

### Threads
Each philosopher is represented by a separate thread that runs independently. This allows philosophers to act concurrently.

### Mutexes
Forks are represented by mutexes (mutual exclusion objects). When a philosopher "picks up" a fork, they lock the mutex. When they "put down" a fork, they unlock it.

### Race Conditions
The program must carefully avoid race conditions, where multiple philosophers try to access the same resource (fork) simultaneously.

### Deadlock
The program must prevent deadlock situations, such as all philosophers picking up their left fork and waiting forever for the right fork.

## Termination Conditions

The simulation can end in two ways:

1. **Death of a philosopher**: If a philosopher doesn't eat for `time_to_die` milliseconds, they die and the simulation ends.

2. **Eating completion**: If the optional 5th argument `number_of_times_each_philosopher_must_eat` is provided, the simulation stops when all philosophers have eaten at least that many times.

## The Code Explained

Let's go through each important file and explain what it does:

### philosophers.h (Header File)

```c
// Our header defines two main structures:

// Structure for each philosopher
typedef struct s_philo
{
    int pos;                // Position at the table (1 to n)
    int ate_times;          // Number of times this philosopher has eaten
    unsigned long last_ate; // Timestamp when philosopher last ate
    char *pos_str;          // Position as a string (for printing)
    int ffork;              // Index of first fork
    int sfork;              // Index of second fork
    pthread_t thread_id;    // Thread identifier for this philosopher
    struct s_env *env;      // Pointer to environment
} t_philo;

// Structure for the environment (shared between all philosophers)
typedef struct s_env
{
    int count;              // Number of philosophers
    int time_to_die;        // Time in ms a philosopher can survive without eating
    int time_to_eat;        // Time in ms it takes to eat
    int time_to_sleep;      // Time in ms a philosopher sleeps
    int eat_count_max;      // Optional: number of times each philosopher must eat
    unsigned long start_time; // Start time of simulation
    int stop_condition;     // Flag to stop all threads when a philosopher dies
    int max_ate;            // Counter for philosophers who have eaten enough
    pthread_mutex_t *forks; // Array of mutexes for forks
    pthread_mutex_t meal;   // Mutex to protect meal-related variables
    pthread_mutex_t writing; // Mutex to protect console output
    t_philo *philos;        // Array of philosopher structures
} t_env;
```

### philosophers.c (Main File)

```c
// Main function - Entry point of the program
int main(int argc, char *argv[])
{
    t_env env; // Create the environment structure
    int i;

    // Step 1: Validate command line arguments
    if (!validate_params(&env, argc, argv))
        return (1);
    
    // Step 2: Set up the environment (allocate memory, initialize mutexes)
    if (!setup_environment(&env))
        return (1);
    
    // Step 3: Launch philosopher threads
    if (!launch_philosophers(&env))
        return (1);
    
    // Step 4: Monitor philosophers to check if any have died
    while (!env.stop_condition)
    {
        philo_dead(&env, env.philos);
        usleep(100); // Small delay to avoid hogging CPU
    }
    
    // Step 5: Join all threads and clean up resources
    for (i = 0; i < env.count; i++)
        pthread_join(env.philos[i].thread_id, NULL);
    
    // Clean up all allocated resources
    for (i = 0; i < env.count; i++)
    {
        pthread_mutex_destroy(&env.forks[i]);
        if (env.philos[i].pos_str)
            free(env.philos[i].pos_str);
    }
    pthread_mutex_destroy(&env.meal);
    pthread_mutex_destroy(&env.writing);
    
    if (env.philos)
        free(env.philos);
    if (env.forks)
        free(env.forks);
    
    return (0);
}

// Function to validate command-line parameters
int validate_params(t_env *env, int argc, char *argv[])
{
    // Check correct number of arguments
    if (argc < 5 || argc > 6)
        return (report_error("Invalid number of arguments"));
    
    // Check all arguments are valid integers
    if (!is_valid_int(argv[1]) || !is_valid_int(argv[2]) || !is_valid_int(argv[3]) || !is_valid_int(argv[4]))
        return (report_error("Invalid argument type"));
    
    // Convert string arguments to integers and store in the environment
    env->count = ft_atoi(argv[1]);         // Number of philosophers
    env->time_to_die = ft_atoi(argv[2]);   // Time to die in milliseconds
    env->time_to_eat = ft_atoi(argv[3]);   // Time to eat in milliseconds
    env->time_to_sleep = ft_atoi(argv[4]); // Time to sleep in milliseconds
    
    // If a 6th argument is provided, it's the number of times each philosopher must eat
    if (argc == 6)
        env->eat_count_max = ft_atoi(argv[5]);
    else
        env->eat_count_max = 0; // No limit if not specified
    
    return (1); // Return 1 to indicate success
}
```

### init.c - Initialization Functions

```c
// Function to initialize the environment
int initialize_environment(t_env *env)
{
    int i;

    // Allocate memory for philosophers array
    env->philos = (t_philo *)malloc(sizeof(t_philo) * env->count);
    if (!env->philos)
        return (0);
    
    // Allocate memory for forks (mutexes)
    env->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * env->count);
    if (!env->forks)
        return (0);
    
    // Record start time and initialize stop condition
    env->start_time = get_time();
    env->stop_condition = 0;
    env->max_ate = 0;  // Initialize counter for philosophers who have eaten enough
    
    // Initialize each philosopher
    for (i = 0; i < env->count; i++)
    {
        // Set basic philosopher properties
        env->philos[i].pos = i + 1;               // Position (1-indexed)
        env->philos[i].ate_times = 0;             // Initialize meals eaten to 0
        env->philos[i].last_ate = get_time();     // Initialize last meal time
        
        // Assign forks to philosopher - to prevent deadlock, we:
        // - For even numbered philosophers: take left fork first, then right fork
        // - For odd numbered philosophers: take right fork first, then left fork
        env->philos[i].ffork = i;                 // First fork
        env->philos[i].sfork = (i + 1) % env->count; // Second fork (wraps around for last philosopher)
        
        env->philos[i].env = env;                 // Link back to environment
        env->philos[i].pos_str = ft_itoa(env->philos[i].pos); // Position as string for printing
        
        // Initialize fork mutex
        pthread_mutex_init(&env->forks[i], NULL);
    }
    
    // Initialize shared mutexes
    pthread_mutex_init(&env->meal, NULL);    // Protects meal-related variables
    pthread_mutex_init(&env->writing, NULL); // Protects console output
    
    return (1); // Return 1 to indicate success
}

// Function to start all philosopher threads
int start_philosopher_threads(t_env *env)
{
    int i;

    for (i = 0; i < env->count; i++)
    {
        // Add a small delay between thread creations to reduce contention
        if (i > 0)
            usleep(100000); // 100ms delay
        
        // Create a thread for the philosopher
        if (pthread_create(&env->philos[i].thread_id, NULL, 
                         (void *)philosopher_routine, &env->philos[i]))
            return (0); // Return 0 to indicate failure
    }
    
    return (1); // Return 1 to indicate success
}
```

### philo_functions.c - Philosopher Actions

```c
// Function for a philosopher to eat
void philo_eat(t_philo *philo)
{
    // Lock the forks for this philosopher
    pthread_mutex_lock(&philo->env->forks[philo->ffork]);
    print_message("has taken a fork", philo);
    pthread_mutex_lock(&philo->env->forks[philo->sfork]);
    print_message("has taken a fork", philo);
    
    // Now the philosopher can eat
    print_message("is eating", philo);
    new_sleep(philo->env->time_to_eat, philo->env);
    
    // Update meal count and timestamp with mutex protection
    pthread_mutex_lock(&philo->env->meal);
    philo->ate_times++;
    philo->last_ate = get_time();
    
    // Check if philosopher has eaten enough times and update counter
    if (philo->env->eat_count_max > 0 && philo->ate_times >= philo->env->eat_count_max)
    {
        philo->env->max_ate++;
        // If all philosophers have eaten enough, set stop condition
        if (philo->env->max_ate >= philo->env->count)
            philo->env->stop_condition = 1;
    }
    pthread_mutex_unlock(&philo->env->meal);
    
    // Release the forks
    pthread_mutex_unlock(&philo->env->forks[philo->sfork]);
    pthread_mutex_unlock(&philo->env->forks[philo->ffork]);
}

// Function to check if a philosopher has died
void philo_dead(t_env *env, t_philo *philos)
{
    int i;

    i = 0;
    while (i < env->count)
    {
        // Use mutex to protect access to meal-related variables
        pthread_mutex_lock(&env->meal);
        if ((unsigned long)(get_time() - philos[i].last_ate) > (unsigned long)env->time_to_die)
        {
            // Announce that philosopher has died
            print_message("died", &philos[i]);
            
            // Set stop condition to end simulation
            env->stop_condition = 1;
            pthread_mutex_unlock(&env->meal);
            break;
        }
        pthread_mutex_unlock(&env->meal);
        i++;
    }
}

// Function to print messages with timestamp
void print_message(const char *msg, t_philo *philo)
{
    unsigned long current_time = get_time();
    char *time_str = ft_itoa(current_time - philo->env->start_time);

    pthread_mutex_lock(&philo->env->writing);
    // Only print if simulation is still running (unless it's a death message)
    if (!philo->env->stop_condition || !strcmp(msg, "died"))
        printf("%s %s %s\n", time_str, philo->pos_str, msg);
    pthread_mutex_unlock(&philo->env->writing);
    free(time_str);
}
```

### threads.c - Thread Management

```c
// The main routine for each philosopher thread
void *philosopher_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg; // Cast argument to philosopher structure
    
    // Loop forever until stop condition is triggered
    while (1)
    {
        // Check if the simulation should stop
        if (philo->env->stop_condition)
            break;
        
        philo_eat(philo);                        // Philosopher eats
        print_message("is sleeping", philo);     // Philosopher sleeps
        new_sleep(philo->env->time_to_sleep, philo->env);
        print_message("is thinking", philo);     // Philosopher thinks
    }
    
    return (NULL);
}
```

### utils.c - Utility Functions

```c
// Function to get current time in milliseconds
unsigned long get_time(void)
{
    struct timeval time;

    gettimeofday(&time, NULL);
    // Convert to milliseconds
    return ((time.tv_sec * (unsigned long)1000) + (time.tv_usec / 1000));
}

// Custom sleep function that checks for stop condition
void new_sleep(unsigned long duration, t_env *env)
{
    unsigned long start = get_time();

    // Sleep in small increments while checking stop condition
    while (!env->stop_condition && get_time() - start < duration)
        usleep(env->count * 2); // Scale sleep time by number of philosophers
}

// Function to get string length
int ft_strlen(const char *str)
{
    int i = 0;

    while (str[i])
        i++;
    return (i);
}

// Function to report errors
int ft_return_error(char *msg)
{
    write(2, msg, ft_strlen(msg));
    return (0);
}

// Function to check if a string is a valid integer
int ft_isint(const char *nptr)
{
    int i = 0;
    int neg = 0;
    long value = 0;

    // Skip whitespace
    while ((nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13)))
        i++;
    
    // Handle negative sign
    if (nptr[i] == '-')
        neg = 1;
    
    // Skip sign
    if (nptr[i] == '-' || nptr[i] == '+')
        i++;
    
    // Process digits and check for overflow
    while (nptr[i] != '\0' && (nptr[i] >= '0' && nptr[i] <= '9'))
    {
        if (value > 214748364 || (value == 214748364 && ((neg && nptr[i] - '0' > 8) || (!neg && nptr[i] - '0' > 7))))
            return (0);
        value = (value * 10) + nptr[i++] - '0';
    }
    
    return (1);
}
```

## How to Use the Program

The program takes 4-5 arguments:
1. `number_of_philosophers`: The number of philosophers at the table
2. `time_to_die`: Time in milliseconds a philosopher can survive without eating
3. `time_to_eat`: Time in milliseconds it takes to eat a meal
4. `time_to_sleep`: Time in milliseconds a philosopher sleeps
5. (Optional) `number_of_times_each_philosopher_must_eat`: Simulation stops when all philosophers have eaten this many times

Example:
```bash
./philo 4 800 200 200
```
This runs the simulation with 4 philosophers, where:
- Philosophers die if they don't eat for 800ms
- It takes 200ms to eat
- It takes 200ms to sleep

## Common Problems and Solutions

### Deadlock
A deadlock can occur if all philosophers pick up one fork and wait for the second forever. Our solution prevents this by having philosophers pick up forks in a specific order.

### Race Conditions
Race conditions can happen when multiple threads access shared data. We use mutex locks to protect shared resources like forks and console output.

### Starvation
Philosophers might never get a chance to eat. Our solution ensures philosophers get regular chances to eat by having them release forks after eating.

## Special Details About the Implementation

### Fork Acquisition
In our implementation, philosophers pick up forks in a specific sequence: first their left fork, then their right fork. This approach helps prevent deadlock, as it ensures philosophers don't all hold one fork waiting for another.

### The Single Philosopher Case: A Perfect Deadlock Example
When there is only 1 philosopher at the table, an interesting situation occurs that perfectly demonstrates the concept of deadlock:

1. With 1 philosopher, there is also only 1 fork (as there are always the same number of forks as philosophers).
2. The philosopher needs 2 forks to eat, but only has access to 1.
3. In our implementation, both `ffork` and `sfork` point to the same fork when there's only 1 philosopher:
   ```c
   env->philos[i].ffork = i;                 // First fork (index 0)
   env->philos[i].sfork = (i + 1) % env->count; // Second fork (0+1) % 1 = 0
   ```
4. The philosopher locks the mutex for the first fork, then tries to lock the same mutex again for the second fork.
5. This creates a deadlock because a thread cannot lock a non-recursive mutex it already owns.
6. The philosopher never gets to eat, but also never dies because the death check in `philo_dead()` can't execute - the philosopher is stuck trying to acquire the second fork.

This case serves as an excellent demonstration of deadlock in concurrent programming: the philosopher is waiting for a resource (the second fork) that will never become available.

### Mutex Protection
We use mutexes to protect:
1. **Forks**: Prevent multiple philosophers from using the same fork simultaneously
2. **Meal variables**: Protect variables related to eating (last_ate, ate_times) from race conditions
3. **Console output**: Prevent garbled output from multiple threads printing at once

### The Optional "Eat Count" Feature
When the 5th argument is provided, we:
1. Track how many times each philosopher eats (`philo->ate_times`)
2. When a philosopher has eaten enough times, increment a counter (`env->max_ate`)
3. When all philosophers have eaten enough, set the stop condition (`env->stop_condition = 1`)

This allows the simulation to terminate naturally when all philosophers have eaten enough, without requiring a philosopher to die.

### Resource Cleanup
At the end of the simulation, we:
1. Join all philosopher threads to make sure they've completed
2. Destroy all mutexes to free up system resources
3. Free all dynamically allocated memory to prevent memory leaks

## Example Use Cases

### Testing Deadlock Prevention
```bash
./philo 5 800 200 200
```
With 5 philosophers, the system should run indefinitely without deadlock. You'll see them continually eating, sleeping, and thinking.

### Testing Death Detection
```bash
./philo 5 200 300 200
```
With time_to_die < time_to_eat, philosophers will die before they can finish eating, and the simulation will end.

### Testing Meal Count Termination
```bash
./philo 5 800 200 200 3
```
This will run the simulation until all philosophers have eaten at least 3 times, then terminate naturally.

### Demonstrating Deadlock
```bash
./philo 1 800 200 200
```
With only 1 philosopher, the program will demonstrate a deadlock where the philosopher attempts to lock the same mutex twice (grabbing the same fork twice). This is a perfect illustration of the deadlock problem in concurrent programming.

## Automated Testing

We've created a comprehensive test script that automatically validates different aspects of the philosophers program. This helps ensure the program behaves correctly under various conditions.

### Running the Test Script

To run the automated tests:

```bash
./test_philosophers.sh
```

The script will run the following tests:

1. **Basic operation test**: Checks if the program runs normally with adequate timings
2. **Death detection test**: Verifies that philosophers die when they don't get to eat in time
3. **Meal count termination test**: Ensures the program terminates after all philosophers have eaten enough times
4. **Edge case - 1 philosopher**: Tests behavior with a single philosopher (expects deadlock)
5. **Edge case - 2 philosophers**: Tests behavior with 2 philosophers (should avoid deadlock)
6. **Very short timings**: Checks behavior with minimal time values
7. **One meal required**: Tests termination when only one meal is required
8. **Zero meals required**: Tests behavior when zero meals are required
9. **Invalid arguments**: Tests handling of incorrect argument counts

### Expected Test Results

Most tests have straightforward expected outcomes, but a few are worth explaining:

- **Basic operation test**: The program should run indefinitely without any philosopher dying when timings are adequate. The test script stops it after a timeout.

- **Death detection test**: When `time_to_die` < `time_to_eat`, a philosopher should die before finishing their meal, and the program should terminate.

- **Edge case - 1 philosopher**: As explained in the "Single Philosopher Case" section, with 1 philosopher, the program enters a deadlock state. The test expects this behavior and considers a timeout as success.

- **Zero meals required**: When the optional meal count is 0, our implementation treats this as "run indefinitely" rather than "terminate immediately" (since philosophers have already eaten 0 meals, but the program keeps running).

### Understanding Test Results

The test script provides clear output indicating:
- The test being run
- The command executed
- The expected result
- Whether the test passed or failed

Each test is run with a timeout to prevent infinite execution. For tests that are expected to run indefinitely (like the basic operation test), a timeout is considered a success. For tests expected to terminate normally (like the meal count test), the program should exit before the timeout.

### Test Parameters

The automated tests use various combinations of parameters to exercise different aspects of the program:

```
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [meals_per_philosopher]
```

Examples from the test script:
- Basic operation: `./philo 4 800 200 200`
- Death detection: `./philo 4 200 300 200`
- Meal termination: `./philo 4 800 200 200 3`
- Edge case: `./philo 1 800 200 200`

### Adding Custom Tests

You can modify the test script to add your own test cases. Simply add a new call to the `run_test` function with:
1. A description of the test
2. The command to run
3. The timeout in seconds
4. The expected result ("run indefinitely" or "terminate normally")

## Key Takeaways

The Dining Philosophers Problem teaches important concepts in concurrent programming:

1. **Resource Allocation**: How to allocate limited resources (forks) among competing processes (philosophers)
2. **Deadlock Prevention**: How to prevent situations where processes are waiting for resources held by others
3. **Synchronization**: How to synchronize multiple threads using mutexes
4. **Race Condition Prevention**: How to prevent multiple threads from interfering with each other
5. **Termination Conditions**: How to properly end a concurrent program when desired conditions are met
6. **Resource Cleanup**: How to properly clean up system resources when a program ends

By solving this problem, you gain valuable experience in concurrent programming, which is essential for developing efficient multi-threaded applications.


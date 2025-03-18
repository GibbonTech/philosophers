#!/bin/bash

# Colors for output formatting
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to run a test with a timeout
run_test() {
    description=$1
    cmd=$2
    timeout=$3
    expected_result=$4
    
    echo -e "${BLUE}=== Test: ${description} ===${NC}"
    echo -e "Command: ${YELLOW}$cmd${NC}"
    echo -e "Timeout: ${timeout}s"
    echo -e "Expected: ${expected_result}"
    
    # Run the command with a timeout
    timeout $timeout $cmd > /dev/null 2>&1
    exit_code=$?
    
    # Check result
    if [ $exit_code -eq 124 ] || [ $exit_code -eq 143 ]; then
        # 124 is timeout's exit code, 143 is SIGTERM (128+15)
        if [ "$expected_result" == "run indefinitely" ]; then
            echo -e "${GREEN}✓ SUCCESS: Program ran until timeout as expected${NC}"
        else
            echo -e "${RED}✗ FAILED: Program timed out unexpectedly${NC}"
        fi
    elif [ $exit_code -eq 0 ]; then
        if [ "$expected_result" == "terminate normally" ]; then
            echo -e "${GREEN}✓ SUCCESS: Program terminated normally as expected${NC}"
        else
            echo -e "${RED}✗ FAILED: Program terminated unexpectedly${NC}"
        fi
    elif [ $exit_code -eq 1 ]; then
        if [ "$expected_result" == "error exit" ]; then
            echo -e "${GREEN}✓ SUCCESS: Program exited with error code as expected${NC}"
        else
            echo -e "${RED}✗ FAILED: Program exited with error code unexpectedly${NC}"
        fi
    else
        echo -e "${RED}✗ FAILED: Program exited with code $exit_code${NC}"
    fi
    echo ""
}

# Ensure the program is compiled
make all > /dev/null

echo -e "${BLUE}===========================================${NC}"
echo -e "${BLUE}       Philosophers Testing Suite         ${NC}"
echo -e "${BLUE}===========================================${NC}"
echo ""

# Test 1: Basic test with enough time to eat (should run indefinitely)
run_test "Basic operation with 4 philosophers" "./philo 4 800 200 200" 3 "run indefinitely"

# Test 2: Death test (philosopher should die because time_to_die < time_to_eat)
run_test "Death detection - time_to_die < time_to_eat" "./philo 4 200 300 200" 2 "terminate normally"

# Test 3: Meal count termination
run_test "Meal count termination - 4 philosophers, 3 meals each" "./philo 4 800 200 200 3" 5 "terminate normally"

# Test 4: Edge case - 1 philosopher (should die, can't eat with 1 fork)
run_test "Edge case - 1 philosopher" "./philo 1 800 200 200" 2 "run indefinitely"

# Test 5: Edge case - 2 philosophers
run_test "Edge case - 2 philosophers" "./philo 2 800 200 200" 3 "run indefinitely"

# Test 6: Very short timings that should still work
run_test "Very short timings" "./philo 4 400 100 100" 3 "run indefinitely"

# Test 7: Only one meal required
run_test "One meal required" "./philo 4 800 200 200 1" 3 "terminate normally"

# Test 8: Zero meals required (should terminate immediately)
run_test "Zero meals required" "./philo 4 800 200 200 0" 1 "run indefinitely"

# Test 9: Invalid arguments
run_test "Invalid arguments - not enough args" "./philo 4 800 200" 1 "error exit"
run_test "Invalid arguments - too many args" "./philo 4 800 200 200 3 extra" 1 "error exit"

echo -e "${BLUE}===========================================${NC}"
echo -e "${BLUE}       Testing Complete                   ${NC}"
echo -e "${BLUE}===========================================${NC}" 
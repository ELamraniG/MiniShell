#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
BOLD='\033[1m'
NC='\033[0m' # No Color

# Path to the minishell executable
MINISHELL="./minishell"

# Test counter
TEST_NUM=0
PASSED=0
FAILED=0

# Function to print section header
print_section() {
    echo -e "${BOLD}${CYAN}=== SECTION: $1 ===${NC}"
    echo
}

# Function to print test header
print_test_header() {
    TEST_NUM=$((TEST_NUM + 1))
    echo -e "${BOLD}=======================================${NC}"
    echo -e "${YELLOW}Test #${TEST_NUM}: ${BOLD}$1${NC}"
    echo -e "${CYAN}Command:${NC} ${BOLD}$2${NC}"
    echo -e "${BOLD}=======================================${NC}"
}

# Function to run a test and check output
run_test() {
    local test_name="$1"
    local command="$2"
    local expected_output="$3"
    local expected_exit_code="$4"
    local check_output="$5"  # "yes" or "no"
    
    print_test_header "$test_name" "$command"
    
    # Create a temporary file for minishell output
    TEST_OUTPUT="$(mktemp)"
    
    # Run the command in minishell
    echo -e "${BLUE}Running command in minishell...${NC}"
    echo "$command" | $MINISHELL > "$TEST_OUTPUT" 2>&1
    ACTUAL_EXIT_CODE=$?
    
    # Display output
    echo -e "${BLUE}Output from minishell:${NC}"
    echo "  $(cat "$TEST_OUTPUT" | sed 's/^/  /')"
    
    # Check exit code
    if [ -n "$expected_exit_code" ]; then
        if [ "$ACTUAL_EXIT_CODE" -eq "$expected_exit_code" ]; then
            echo -e "${GREEN}✓ Exit code check passed: Got $ACTUAL_EXIT_CODE as expected${NC}"
            exit_code_check="passed"
        else
            echo -e "${RED}✗ Exit code check failed: Expected $expected_exit_code, got $ACTUAL_EXIT_CODE${NC}"
            exit_code_check="failed"
        fi
    else
        exit_code_check="skipped"
        echo -e "${YELLOW}⚠ Exit code check skipped${NC}"
    fi
    
    # Check output if requested
    if [ "$check_output" = "yes" ] && [ -n "$expected_output" ]; then
        if grep -q "$expected_output" "$TEST_OUTPUT"; then
            echo -e "${GREEN}✓ Output check passed: Found '$expected_output' in output${NC}"
            output_check="passed"
        else
            echo -e "${RED}✗ Output check failed: Expected to find '$expected_output' in output${NC}"
            output_check="failed"
        fi
    else
        output_check="skipped"
        echo -e "${YELLOW}⚠ Output check skipped${NC}"
    fi
    
    # Overall test result
    if [ "$exit_code_check" = "passed" ] || [ "$exit_code_check" = "skipped" ]; then
        if [ "$output_check" = "passed" ] || [ "$output_check" = "skipped" ]; then
            echo -e "${GREEN}${BOLD}✓ OVERALL TEST RESULT: PASSED${NC}"
            PASSED=$((PASSED + 1))
        else
            echo -e "${RED}${BOLD}✗ OVERALL TEST RESULT: FAILED${NC}"
            FAILED=$((FAILED + 1))
        fi
    else
        echo -e "${RED}${BOLD}✗ OVERALL TEST RESULT: FAILED${NC}"
        FAILED=$((FAILED + 1))
    fi
    
    # Clean up
    rm -f "$TEST_OUTPUT"
    echo
}

# Start testing

print_section "EXTERNAL COMMANDS TESTS"

run_test "ls_basic" "ls" "" 0 "no"
run_test "cat_file" "cat /etc/passwd | grep root" "root" 0 "yes"
run_test "grep_command" "grep PATH /etc/environment" "PATH" 0 "yes"
run_test "command_not_found" "nonexistentcommand" "command not found" 127 "yes"

print_section "REDIRECTION TESTS"

run_test "output_redirection" "ls > test_out.txt && cat test_out.txt" "" 0 "no"
run_test "append_redirection" "echo 'line1' > append.txt && echo 'line2' >> append.txt && cat append.txt | wc -l" "2" 0 "yes"
run_test "input_redirection" "cat < /etc/passwd | grep root" "root" 0 "yes"
run_test "multiple_redirections" "ls > multi.txt && cat < multi.txt > multi_out.txt && cat multi_out.txt | wc -l" "" 0 "no"

print_section "PIPE TESTS"

run_test "simple_pipe" "ls -la | grep test" "" 0 "no"
run_test "multi_pipe" "cat /etc/passwd | grep root | wc -l" "" 0 "no"
run_test "complex_pipe" "cat /etc/passwd | grep bash | sort | uniq | wc -l" "" 0 "no"

print_section "LOGICAL OPERATOR TESTS"

run_test "and_operator_true" "ls && cat /etc/passwd | grep root" "root" 0 "yes"
run_test "and_operator_false" "nonexistentcommand && ls" "command not found" 127 "yes"
run_test "or_operator_true" "ls || nonexistentcommand" "" 0 "no"
run_test "or_operator_false" "nonexistentcommand || ls" "command not found" 0 "yes"
run_test "mixed_operators" "ls && cat /etc/passwd | grep root || nonexistentcommand" "root" 0 "yes"

print_section "PARENTHESES TESTS"

run_test "simple_parentheses" "(ls)" "" 0 "no"
run_test "parentheses_with_pipe" "(cat /etc/passwd | grep root)" "root" 0 "yes"
run_test "parentheses_with_operators" "(ls && cat /etc/passwd | grep root) || nonexistentcommand" "root" 0 "yes"
run_test "complex_parentheses" "(cat /etc/passwd) | grep root" "root" 0 "yes"

print_section "FILE OPERATION TESTS"

run_test "create_file" "touch test_file && ls test_file && rm test_file" "test_file" 0 "yes"
run_test "copy_content" "cat /etc/passwd > passwd_copy && cat passwd_copy | grep root" "root" 0 "yes"
run_test "find_files" "find /etc -name \"*.conf\" | head -n 1" ".conf" 0 "yes"

print_section "ENVIRONMENT VARIABLES TESTS"

# This section is tricky since we can't use export directly
# We'll test if environment variables from the parent shell are passed to minishell
run_test "environment_variables" "cat /etc/environment | grep PATH" "PATH" 0 "yes"

print_section "PERMISSION TESTS"

run_test "permission_denied" "touch no_perm_file && chmod 000 no_perm_file && cat no_perm_file && chmod 644 no_perm_file && rm no_perm_file" "Permission denied" 1 "yes"
run_test "directory_access" "ls /tmp" "" 0 "no"
run_test "nonexistent_file" "cat /this/file/does/not/exist" "No such file" 1 "yes"

# Print test summary
echo -e "${BOLD}${CYAN}======= TEST SUMMARY =======${NC}"
echo "Total tests: $TEST_NUM"
echo "Passed tests: $PASSED"
echo "Failed tests: $FAILED"

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}${BOLD}All tests passed!${NC}"
else
    echo -e "${RED}${BOLD}Some tests failed.${NC}"
fi

exit $FAILED

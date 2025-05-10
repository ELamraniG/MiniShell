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
    cat "$TEST_OUTPUT" | sed 's/^/  /'
    
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

print_section "JOINED COMMAND TESTS"

run_test "ls_joined" "ls'-la'" "command not found" 127 "yes"
run_test "ls_joined_complex" "ls'-l''-a'" "command not found" 127 "yes"
run_test "echo_joined" "echo'hello'" "command not found" 127 "yes"

print_section "COMMAND WITH SPACES TESTS"

run_test "ls_space" "ls -la" "" 0 "no"
run_test "echo_space" "echo hello" "hello" 0 "yes"
run_test "cat_space" "cat /etc/passwd" "root" 0 "yes"

print_section "QUOTE HANDLING TESTS"

run_test "single_quote_test" "echo 'hello world'" "hello world" 0 "yes"
run_test "double_quote_test" "echo \"hello world\"" "hello world" 0 "yes"
run_test "mixed_quotes" "echo 'hello' \"world\"" "hello world" 0 "yes"
run_test "quotes_in_quotes" "echo '\"hello\"'" "\"hello\"" 0 "yes"

print_section "REDIRECTIONS WITH COMPLEX COMMANDS"

run_test "redirect_with_space" "ls -la > test_out.txt && cat test_out.txt && rm test_out.txt" "" 0 "no"
run_test "redirect_append" "echo hello >> test_append.txt && echo world >> test_append.txt && cat test_append.txt && rm test_append.txt" "hello\nworld" 0 "yes"
run_test "redirect_in" "echo 'test data' > test_in.txt && cat < test_in.txt && rm test_in.txt" "test data" 0 "yes"
run_test "redirect_complex" "ls -la > test1.txt && cat < test1.txt > test2.txt && cat test2.txt && rm test1.txt test2.txt" "" 0 "no"

print_section "PIPE TESTS WITH COMPLEX COMMANDS"

run_test "pipe_basic" "ls -la | grep test" "" 0 "no"
run_test "pipe_multiple" "ls -la | grep test | wc -l" "" 0 "no"
run_test "pipe_with_redirects" "cat /etc/passwd | grep root > root.txt && cat root.txt && rm root.txt" "root" 0 "yes"

print_section "LOGICAL OPERATORS TESTS"

run_test "and_operator" "ls && echo success" "success" 0 "yes"
run_test "or_operator" "non_existent_command || echo fallback" "fallback" 0 "yes"
run_test "complex_logical" "ls -la && cat /etc/passwd | grep root || echo error" "root" 0 "yes"

print_section "PARENTHESES TESTS"

run_test "parentheses_basic" "(ls)" "" 0 "no"
run_test "parentheses_pipe" "(ls -la | grep test)" "" 0 "no"
run_test "parentheses_nested" "((ls -la))" "" 0 "no"
run_test "parentheses_operators" "(ls -la && echo inside) | grep inside" "inside" 0 "yes"

print_section "SPECIAL CHARACTER TESTS"

run_test "semicolon_split" "echo hello; echo world" "hello\nworld" 0 "yes"
run_test "background_job" "sleep 1 &" "" 0 "no"
run_test "special_chars" "echo \$PATH" "\$PATH" 0 "yes"
run_test "special_escape" "echo \\\\n" "\\n" 0 "yes"

print_section "COMPLEX REAL-WORLD EXAMPLES"

run_test "find_with_grep" "find /etc -type f -name \"*.conf\" | head -n 1" ".conf" 0 "yes"
run_test "complex_pipeline" "cat /etc/passwd | grep root | cut -d: -f1" "root" 0 "yes"
run_test "permission_test" "touch test_perm && chmod 000 test_perm && cat test_perm || echo 'Permission issue' && chmod 644 test_perm && rm test_perm" "Permission issue" 0 "yes"
run_test "command_substitution" "echo \"Today is $(date +%A)\"" "Today is" 0 "yes"

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

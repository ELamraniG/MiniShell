#!/bin/bash

# Colors for better output
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

# Function to run a test and check exit code
run_test() {
    local test_name="$1"
    local command="$2"
    local expected_exit_code="$3"
    
    print_test_header "$test_name" "$command"
    
    # Create a temporary file for minishell output
    TEST_OUTPUT="$(mktemp)"
    
    echo -e "${BLUE}Running command in minishell...${NC}"
    
    # Run the command in minishell
    echo "$command" | $MINISHELL > "$TEST_OUTPUT" 2>&1
    ACTUAL_EXIT_CODE=$?
    
    # Display output
    echo -e "${BLUE}Output from minishell:${NC}"
    cat "$TEST_OUTPUT" | sed 's/^/  /'
    
    # For all tests, print the actual exit code explicitly for debugging
    echo -e "${BOLD}Actual exit code from minishell: $ACTUAL_EXIT_CODE${NC}"
    
    # Check exit code
    if [ "$ACTUAL_EXIT_CODE" -eq "$expected_exit_code" ]
    then
        echo -e "${GREEN}✓ Exit code check passed: Got $ACTUAL_EXIT_CODE as expected${NC}"
        echo -e "${GREEN}${BOLD}✓ OVERALL TEST RESULT: PASSED${NC}"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}✗ Exit code check failed: Expected $expected_exit_code, got $ACTUAL_EXIT_CODE${NC}"
        echo -e "${RED}${BOLD}✗ OVERALL TEST RESULT: FAILED${NC}"
        FAILED=$((FAILED + 1))
    fi
    
    # Clean up
    rm -f "$TEST_OUTPUT"
    echo
}

# Create test files
echo "test data" > test_input.txt
touch test_input_noperm.txt
chmod 000 test_input_noperm.txt

print_section "BASIC PIPELINE ERROR HANDLING"

run_test "first_cmd_not_found" "notexistcmd | wc -l" 0
run_test "middle_cmd_not_found" "ls | notexistcmd | wc -l" 0
run_test "last_cmd_not_found" "ls | grep test | notexistcmd" 127
run_test "all_cmds_not_found" "notexist1 | notexist2 | notexist3" 127

print_section "PERMISSION ISSUES IN PIPELINES"

run_test "permission_denied_input" "cat test_input_noperm.txt | grep test" 1
run_test "permission_denied_output" "echo test > output_noperm.txt 2>/dev/null | wc -l" 0

print_section "REDIRECTION ERRORS IN PIPELINES"

run_test "input_redir_nonexistent" "cat < nonexistentfile | grep test" 1
run_test "complex_redir_errors" "cat < nonexistentfile | grep test > output.txt 2>/dev/null" 1

print_section "COMPLEX PIPELINE ERROR CASES"

run_test "long_pipeline_middle_error" "ls | grep test | nonexistcmd | sort | uniq | wc -l" 0
run_test "pipeline_with_subshell_error" "ls | (nonexistcmd) | wc -l" 0
run_test "nested_pipelines_error" "(ls | nonexistcmd) | grep test" 0
run_test "logical_with_pipeline_error" "nonexistcmd && (ls | grep test) || echo recovery" 0
run_test "pipeline_with_logical_error" "ls | grep test && nonexistcmd | wc -l" 127

print_section "RESOURCE HANDLING IN PIPELINES"

run_test "deep_pipeline" "ls | grep a | grep b | grep c | grep d | grep e | grep f | grep g | grep h" 0
run_test "multiple_redirections" "ls > out1 | grep test > out2 | wc -l > out3 && cat out1 out2 out3 && rm out1 out2 out3" 0

print_section "INTERRUPTION HANDLING"

run_test "pipeline_with_closed_stream" "cat file_doesnt_exist | head -n 1 | wc -l" 1
run_test "cat_with_no_arg" "cat | head -n 1" 0 # Might need manual CTRL+D in real use

print_section "SPECIAL OUTPUT ERRORS"

run_test "stderr_redirection" "ls -Z 2>&1 | wc -l" 0
run_test "mixed_stdout_stderr" "ls . missing_dir 2>&1 | grep -v ." 0

# Clean up
chmod 644 test_input_noperm.txt
rm -f test_input.txt test_input_noperm.txt out1 out2 out3 2>/dev/null

# Print test summary
echo -e "${BOLD}${CYAN}======= TEST SUMMARY =======${NC}"
echo "Total tests: $TEST_NUM"
echo "Passed tests: $PASSED"
echo "Failed tests: $FAILED"

if [ $FAILED -eq 0 ]
then
    echo -e "${GREEN}${BOLD}All tests passed!${NC}"
else
    echo -e "${RED}${BOLD}Some tests failed.${NC}"
fi

exit $FAILED

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
    if [ "$check_output" = "yes" && [ -n "$expected_output" ]; then
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

print_section "COMPLEX REDIRECTION TESTS"

# Test multiple redirections in a row
run_test "multiple_redirections_chain" "ls > file1.txt > file2.txt > file3.txt && cat file3.txt && rm file1.txt file2.txt file3.txt" "" 0 "no"

# Test redirection to a file that doesn't have parent directory
run_test "redirection_nonexistent_parent" "ls > /nonexistent/dir/file.txt" "No such file or directory" 1 "yes"

# Test redirecting to a device file
run_test "redirection_to_device" "ls > /dev/null && echo Success" "Success" 0 "yes"

# Test redirecting from a nonexistent file
run_test "redirection_from_nonexistent" "cat < nonexistentfile.txt" "No such file or directory" 1 "yes"

# Test append redirection
run_test "append_test" "echo line1 > append_test.txt && echo line2 >> append_test.txt && cat append_test.txt && rm append_test.txt" "line1" 0 "yes"

print_section "COMPLEX PIPE TESTS"

# Test piping through multiple commands
run_test "complex_pipe_chain" "cat /etc/passwd | grep r | sort | uniq | wc -l" "" 0 "no"

# Test pipe with parentheses
run_test "pipe_with_parentheses" "(cat /etc/passwd | grep root) | wc -l" "2" 0 "yes"

# Test with pipes and redirections mixed
run_test "pipe_with_redirections" "cat /etc/passwd > tmp1.txt && cat tmp1.txt | grep root > tmp2.txt && cat tmp2.txt && rm tmp1.txt tmp2.txt" "root" 0 "yes"

print_section "LOGICAL OPERATOR EDGE CASES"

# Test complex expression with multiple operators
run_test "complex_logical_chain" "false && echo 1 || (true && echo 2 || echo 3) && echo 4" "2" 0 "yes"

# Test with nonexistent commands and logical operators
run_test "logical_with_nonexistent" "nonexistent1 || nonexistent2 || ls" "" 0 "no"

# Test with command that fails with non-zero exit code
run_test "logical_with_failure" "grep nonexistent /etc/passwd && echo Success || echo Failed" "Failed" 0 "yes"

print_section "PARENTHESES EDGE CASES"

# Test deeply nested parentheses
run_test "nested_parentheses" "((((ls -la | grep test)))) | wc -l" "" 0 "no"

# Test parentheses with multiple commands and redirections
run_test "parentheses_with_redirections" "(ls > tmp1.txt && cat < tmp1.txt > tmp2.txt) && cat tmp2.txt && rm tmp1.txt tmp2.txt" "" 0 "no"

# Test parentheses with logical operators
run_test "parentheses_with_logical" "(false || true) && echo Success" "Success" 0 "yes"

print_section "FILENAME AND PATH EDGE CASES"

# Test with filenames containing special characters
run_test "special_char_filename" "touch 'file with spaces.txt' && ls 'file with spaces.txt' && rm 'file with spaces.txt'" "file with spaces.txt" 0 "yes"

# Test with very long filenames
run_test "long_filename" "touch verylongfilenamexxxxxxxxxxxxxxxxxxxxx.txt && ls verylongfilenamexxxxxxxxxxxxxxxxxxxxx.txt && rm verylongfilenamexxxxxxxxxxxxxxxxxxxxx.txt" "verylongfilenamexxxxxxxxxxxxxxxxxxxxx.txt" 0 "yes"

# Test with a path containing spaces
run_test "path_with_spaces" "mkdir -p 'dir with spaces' && touch 'dir with spaces/file.txt' && ls 'dir with spaces' && rm -r 'dir with spaces'" "file.txt" 0 "yes"

print_section "ERROR HANDLING TESTS"

# Test with many arguments (but not using sequence generation)
run_test "many_args" "ls -la /etc /tmp /usr /bin /home" "" 0 "no"

# Test with very large output
run_test "large_output" "cat /etc/passwd /etc/passwd /etc/passwd /etc/passwd | cat | cat | cat" "" 0 "no"

# Test interrupting a command with file descriptors
run_test "interrupted_redirect" "ls > tempfile.txt || true && cat tempfile.txt && rm tempfile.txt" "" 0 "no"

print_section "EDGE CASE COMBINATIONS"

# Test complex nested constructs
run_test "complex_nested" "(ls -la | grep test) && ((cat /etc/passwd | grep root) > out.txt) && cat out.txt && rm out.txt" "" 0 "no"

# Test with multiple input/output redirections and pipes
run_test "multiple_ios" "cat < /etc/passwd > tmp1.txt && cat < tmp1.txt | grep root > tmp2.txt && cat < tmp2.txt | wc -l && rm tmp1.txt tmp2.txt" "" 0 "no"

# Test with command substitution-like syntax
run_test "pseudo_subst" "(cat /etc/passwd) > tmp.txt && grep root < tmp.txt && rm tmp.txt" "root" 0 "yes"

# Test with empty commands and logical operators
run_test "empty_commands" "true && true && true && ls" "" 0 "no"

print_section "DEEPLY NESTED PARENTHESES TESTS"

# Test with deeply nested parentheses and redirections
run_test "extreme_nesting" "(((ls -la) | grep test) | (wc -l) > output1.txt) && cat output1.txt && rm output1.txt" "" 0 "no"

# Test with multiple nested parentheses and complex piping
run_test "nested_with_multiple_pipes" "((ls -la | grep c) && (cat /etc/passwd | grep root)) | wc -l" "" 0 "no"

# Test with nested parentheses and logical operators
run_test "nested_with_logic" "((false || true) && (ls -la | grep test)) | wc -l" "" 0 "no" 

print_section "COMPLEX REDIRECTION CHAINS"

# Multiple redirections with pipes in between
run_test "pipe_redirect_mix" "cat /etc/passwd > file1.txt && (cat file1.txt | grep root > file2.txt) && cat file2.txt && rm file1.txt file2.txt" "root" 0 "yes"

# Redirecting output of a complex command
run_test "complex_output_redirect" "((ls -la | grep test) && cat /etc/passwd | grep root) > outfile.txt && cat outfile.txt && rm outfile.txt" "" 0 "no"

# Multiple input/output redirections in a complex chain
run_test "input_output_chain" "cat /etc/passwd > infile.txt && (cat < infile.txt | grep root > outfile.txt) && cat outfile.txt && rm infile.txt outfile.txt" "root" 0 "yes"

print_section "LOGICAL OPERATOR COMPLEX CHAINS"

# Multiple logical operators with nested commands
run_test "deep_logical_chain" "(ls -la | grep nonexistent) || ((cat /etc/passwd | grep root) && ls -la > /dev/null)" "root" 0 "yes"

# Testing precedence with a mix of && and ||
run_test "precedence_mix" "false && (ls -la | grep test) || ((cat /etc/passwd | grep root) && ls -la > /dev/null)" "root" 0 "yes"

# Alternating logical operators with complex commands
run_test "alternating_logic" "(false || true) && (ls -la | grep test) || (cat /etc/passwd | grep nonexistent) && (ls nonexistentfile || true)" "" 0 "no"

print_section "COMPLEX PIPE CONSTRUCTS"

# Many pipes in sequence with different commands
run_test "pipe_sequence" "cat /etc/passwd | grep r | sort | uniq | grep -v '#' | head -n 5 | wc -l" "" 0 "no"

# Pipes with redirections in the middle
run_test "pipes_with_redirects" "cat /etc/passwd | grep root > tmp.txt && cat tmp.txt | wc -l && rm tmp.txt" "" 0 "no"

# Pipes within parentheses and across them
run_test "pipes_across_groups" "(cat /etc/passwd | grep root) | (sort | uniq) | wc -l" "" 0 "no"

print_section "FILE PATH EDGE CASES"

# Path with multiple spaces and special characters
run_test "complex_path" "mkdir -p 'dir with   multiple   spaces' && touch 'dir with   multiple   spaces/test file.txt' && ls 'dir with   multiple   spaces' && rm -r 'dir with   multiple   spaces'" "test file.txt" 0 "yes"

# Very long path and filename
run_test "very_long_path" "mkdir -p 'directory_with_a_very_long_name_that_tests_buffer_handling' && touch 'directory_with_a_very_long_name_that_tests_buffer_handling/file_with_a_very_long_name_that_tests_buffer_handling.txt' && ls 'directory_with_a_very_long_name_that_tests_buffer_handling' && rm -r 'directory_with_a_very_long_name_that_tests_buffer_handling'" "" 0 "no"

# Nested directories with spaces and special characters
run_test "nested_dirs_with_spaces" "mkdir -p 'outer space/inner space' && touch 'outer space/inner space/test.txt' && ls 'outer space/inner space' && rm -r 'outer space'" "test.txt" 0 "yes"

print_section "EXTREMELY COMPLEX COMBINED CASES"

# Mixture of everything: redirections, pipes, parentheses, logical operators
run_test "everything_mixed" "((ls -la | grep test) > tmp1.txt || true) && ((cat tmp1.txt | grep -v test) > tmp2.txt || cat /etc/passwd > tmp2.txt) && cat tmp2.txt | grep root && rm tmp1.txt tmp2.txt" "root" 0 "yes"

# Complex error handling with logical operators
run_test "complex_error_handling" "(ls non_existent_dir || echo 'dir not found' > tmp1.txt) && cat tmp1.txt && rm tmp1.txt" "dir not found" 0 "yes"

# Test with a very complex nested structure
run_test "deeply_nested_structure" "((((ls -la | grep test) || (ls -la | grep shell)) && ((cat /etc/passwd | grep root) || (cat /etc/passwd | grep daemon))) | wc -l) > result.txt && cat result.txt && rm result.txt" "" 0 "no"

print_section "SPECIAL FILE HANDLING"

# Using /dev/null in a complex way
run_test "dev_null_complex" "(ls -la > /dev/null && ls -la | grep test) || (cat /etc/passwd | grep root > /dev/null && ls)" "" 0 "no"

# Creating and removing directories in a complex chain
run_test "directory_chain" "mkdir test_dir && touch test_dir/file.txt && ls test_dir > output.txt && cat output.txt && rm -r test_dir && rm output.txt" "file.txt" 0 "yes"

# Working with temporary files across parentheses
run_test "cross_parentheses_files" "(ls -la > temp1.txt) && (cat temp1.txt | grep test > temp2.txt) && cat temp2.txt && rm temp1.txt temp2.txt" "" 0 "no"

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

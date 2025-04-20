#include "../includes/minishell.h"


void	put_syntax_error(void)
{
	write(2, "bash: syntax error near unexpected token\n",
		ft_strlen("bash: syntax error near unexpected token\n"));
	// free_all();
	exit(258);
}

void	arg_type(t_lex_list *token)
{
	if (ft_strcmp(token->s, "(") == 0)
		token->a_type = OP_PAREN;
	else if (ft_strcmp(token->s, ")") == 0)
		token->a_type = CL_PAREN;
	else if (ft_strcmp(token->s, "||") == 0)
		token->a_type = OR;
	else if (ft_strcmp(token->s, "&&") == 0)
		token->a_type = AND;
	else if (ft_strcmp(token->s, "|") == 0)
		token->a_type = PIPE;
	else if (ft_strcmp(token->s, "<<") == 0)
		token->a_type = HEREDOC;
	else if (ft_strcmp(token->s, ">>") == 0)
		token->a_type = APPEND;
	else if (ft_strcmp(token->s, ">") == 0)
		token->a_type = OUT_REDIR;
	else if (ft_strcmp(token->s, "<") == 0)
		token->a_type = IN_REDIR;
	else
		token->a_type = WORD;
}



void	get_next_quote(char *str, int *i, char c)
{
	(*i)++;
	while (str[*i] && str[*i] != c)
		(*i)++;
	if (str[*i] != c)
	{
		// free_all();
	put_syntax_error();
	exit(258);
	}
}
int	is_special(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	if (c == '&' || c == '(' || c == ')')
		return (1);
	return (0);
}

int	is_quote(char c)
{
	if (c == '\'' || c == '"')
		return (1);
	return (0);
}
t_lex_list	*lexing_the_thing(char *str)
{
	int			i;
	int			j;
	t_lex_list	*tokens;
	char		*s;
	int			is_space;
	t_q_flags	quote;

	i = 0;
	is_space = 0;
	i = 0;
	j = 0;
	tokens = NULL;
	while (str[i])
	{
		is_space = 0;
		while (ft_isspace(str[i]))
		{
			j++;
			i++;
		}
		if (str[i] == '\'')
		{
			quote = SQ;
			get_next_quote(str, &i, '\'');
			if (str[i] && ft_isspace(str[i + 1]))
				is_space = 1;
			i++;
		}
		else if (str[i] == '"')
		{
			quote = DQ;
			get_next_quote(str, &i, '"');
			if (str[i] && ft_isspace(str[i + 1]))
				is_space = 1;
			i++;
		}
		else if (is_special(str[i]))
		{
			quote = NQ;
			if ((str[i]) == str[i + 1])
				i++;
			i++;
		}
		else
		{
			quote = NQ;
			while (str[i] && !ft_isspace(str[i]) && !is_special(str[i])
				&& !is_quote(str[i]))
				i++;
			if (str[i] && ft_isspace(str[i]))
				is_space = 1;
		}
		s = ft_substr(str, j, i - j);
		if (s)
			add_to_list(&tokens, s, quote, is_space);
		if (!str[i])
			break ;

		j = i;
	}
	return (tokens);
}

void	set_the_arg_type(t_lex_list *token)
{
	while (token)
	{
		arg_type(token);
		token = token->next;
	}
}




int	is_special_opperand(int n)
{
	if (n != WORD)
		return (1);
	return (0);
}

int	is_special_for_pipe(int n)
{
	// Allow WORD, OP_PAREN, and all redirection types after a pipe
	if (n == WORD || n == OP_PAREN || n == IN_REDIR || 
		n == OUT_REDIR || n == APPEND || n == HEREDOC)
		return (0);
	return (1);
}

int	is_special_for_specials(int n)
{
	// Also allow redirection operators in the special contexts
	if (n == WORD || n == OP_PAREN || n == CL_PAREN || 
		n == IN_REDIR || n == OUT_REDIR || n == APPEND || n == HEREDOC)
		return (0);
	return (1);
	// Also allow redirection operators in the special contexts
	if (n == WORD || n == OP_PAREN || n == CL_PAREN || 
		n == IN_REDIR || n == OUT_REDIR || n == APPEND || n == HEREDOC)
		return (0);
	return (1);
}

int check_valid_parenthesis(t_lex_list *token)
{
	int f = 0;
	while(token)
	{
		if (token->a_type == OP_PAREN)
			f++;
		else if (token->a_type == CL_PAREN)
			f--;
		if (f < 0)
			return 0;
		token = token->next;
	}
	if (f == 0)
		return 1;
	return 0;
}

void ft_putstr_fd(int fd, char *s)
{
	if (s)
		write(fd, s, ft_strlen(s));
}
void put_syntax_error_token(t_lex_list *token)
{
    if (!token) {
        ft_putstr_fd(STDERR_FILENO, "minishell: syntax error near unexpected token `newline'\n");
    } else {
        char *token_str;
        if (token->s) {
            token_str = token->s;
        } else {
            token_str = "";
        } 
        if (token->a_type == AND) {
            ft_putstr_fd(STDERR_FILENO, "minishell: syntax error near unexpected token `&&'\n");
        } else if (token->a_type == OR) {
            ft_putstr_fd(STDERR_FILENO, "minishell: syntax error near unexpected token `||'\n");
        } else if (token->a_type == PIPE) {
            ft_putstr_fd(STDERR_FILENO, "minishell: syntax error near unexpected token `|'\n");
        // } else if (token->a_type == OUT_REDIR || token->a_type == APPEND || 
        //           token->a_type == IN_REDIR || token->a_type == HEREDOC) {
        //     ft_putstr_fd(STDERR_FILENO, "minishell: syntax error near unexpected token `");
        //     ft_putstr_fd(STDERR_FILENO, token_str);
        //     ft_putstr_fd(STDERR_FILENO, "'\n");
        } else {
            ft_putstr_fd(STDERR_FILENO, "minishell: syntax error near unexpected token `");
            ft_putstr_fd(STDERR_FILENO, token_str);
            ft_putstr_fd(STDERR_FILENO, "'\n");
        }
    }
	// change exit with return status later
    exit(258);
}



void handle_syntax_errors(t_lex_list *token) {
    if (!token)
        return;
    
    t_lex_list *current = token;
    if (current->a_type == PIPE || current->a_type == AND || current->a_type == OR)
        return put_syntax_error_token(current);
    if (check_valid_parenthesis(token) == 0)
        return put_syntax_error_token(NULL);
    while (current)
    {
        t_lex_list *next = current->next;
        if ((current->a_type == OUT_REDIR || current->a_type == APPEND ||
             current->a_type == IN_REDIR || current->a_type == HEREDOC) &&
             (!next || (next->a_type != WORD && next->a_type != OP_PAREN)))
        {
            if (next)
                return put_syntax_error_token(next);
            else
                return put_syntax_error_token(NULL);
        }
        if (current->a_type == PIPE && !next)
            return put_syntax_error_token(NULL);
        if (current->a_type == PIPE && next && next->a_type == PIPE)
            return put_syntax_error_token(next);
        if ((current->a_type == AND || current->a_type == OR) &&
             (!next || next->a_type == PIPE || next->a_type == AND || next->a_type == OR))
        {
            if (next)
                return put_syntax_error_token(next);
            else
                return put_syntax_error_token(NULL);
        }
        current = next;
    }
}

// /**
//  * Error reporting function that mimics bash's error format
//  * For '> #file.txt|| && cat file.txt', bash reports "syntax error near unexpected token `newline'"
//  */


// /**
//  * Advanced Minishell Syntax Test Suite
//  * 
//  * This file contains structured tests for your minishell syntax checker.
//  * Now includes tests for unclosed quotes and more complex command structures.
//  */
// /**
//  * Comprehensive Minishell Syntax Test Suite
//  *
//  * This file contains 150 structured tests for your minishell syntax checker.
//  * Each test is designed to thoroughly evaluate your syntax error detection.
//  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <fcntl.h>


typedef struct s_test {
    char    *name;
    char    *cmd;
    int     expected; // 0 for valid syntax, 1 for syntax error (based on run_syntax_test logic)
    char    *description;
} t_test;
#include <fcntl.h>


typedef struct s_test {
    char    *name;
    char    *cmd;
    int     expected; // 0 for valid syntax, 1 for syntax error (based on run_syntax_test logic)
    char    *description;
} t_test;

// Function to print test results with color
void print_test_result(char *name, int result, int expected)
{
    if (result == expected)
        printf("\033[0;32m[PASS]\033[0m %s\n", name);
    else
        printf("\033[0;31m[FAIL]\033[0m %s (got %d, expected %d)\n",
               name, result, expected);
}

int run_syntax_test(char *cmd)
{
	int status;
    pid_t pid;

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE); // Exit test suite if fork fails
    }

    if (pid == 0) {
        // Child process: Run the syntax check
        // Redirect stderr to /dev/null to suppress minishell error messages during tests
        freopen("/dev/null", "w", stderr);
    if (pid == 0) {
        // Child process: Run the syntax check
        // Redirect stderr to /dev/null to suppress minishell error messages during tests
        freopen("/dev/null", "w", stderr);

        t_lex_list *token = lexing_the_thing(cmd);
        if (token == NULL && strlen(cmd) > 0 && strspn(cmd, " \t\n\v\f\r") != strlen(cmd)) {
            // If lexing fails for non-empty/non-whitespace command, consider it a failure
            // Or handle lexing errors explicitly if lexing_the_thing indicates them
            exit(EXIT_FAILURE); // Indicate lexing error
        }
        t_lex_list *token = lexing_the_thing(cmd);
        if (token == NULL && strlen(cmd) > 0 && strspn(cmd, " \t\n\v\f\r") != strlen(cmd)) {
            // If lexing fails for non-empty/non-whitespace command, consider it a failure
            // Or handle lexing errors explicitly if lexing_the_thing indicates them
            exit(EXIT_FAILURE); // Indicate lexing error
        }

        // Handle empty/whitespace commands gracefully (usually valid syntax)
        if (token == NULL) {
             exit(EXIT_SUCCESS); // Empty/whitespace line is syntactically valid
        }
        // Handle empty/whitespace commands gracefully (usually valid syntax)
        if (token == NULL) {
             exit(EXIT_SUCCESS); // Empty/whitespace line is syntactically valid
        }

        set_the_arg_type(token);
        handle_syntax_errors(token); // This function should exit(2) or similar on syntax error
        set_the_arg_type(token);
        handle_syntax_errors(token); // This function should exit(2) or similar on syntax error

        // If handle_syntax_errors returns or doesn't exit, syntax is considered valid
        // Make sure to free tokens if handle_syntax_errors doesn't free on success
        // free_lex_list(token); // Add your token freeing function here if needed
        exit(EXIT_SUCCESS); // No syntax error detected
        // If handle_syntax_errors returns or doesn't exit, syntax is considered valid
        // Make sure to free tokens if handle_syntax_errors doesn't free on success
        // free_lex_list(token); // Add your token freeing function here if needed
        exit(EXIT_SUCCESS); // No syntax error detected

    } else {
        // Parent process: Wait for the child and check status
        waitpid(pid, &status, 0);
    } else {
        // Parent process: Wait for the child and check status
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            // Child exited normally
            // WEXITSTATUS returns the exit code of the child
            // We expect 0 for success (valid syntax) and non-zero for error (invalid syntax)
            return WEXITSTATUS(status) != 0; // Return 1 if exit status is non-zero (error), 0 otherwise
        } else {
            // Process terminated abnormally (e.g., signal)
            printf("\033[0;31m[CRASH]\033[0m Test process for '%s' terminated abnormally.\n", cmd);
            return 1; // Consider abnormal termination as an error
        }
    }
}
        if (WIFEXITED(status)) {
            // Child exited normally
            // WEXITSTATUS returns the exit code of the child
            // We expect 0 for success (valid syntax) and non-zero for error (invalid syntax)
            return WEXITSTATUS(status) != 0; // Return 1 if exit status is non-zero (error), 0 otherwise
        } else {
            // Process terminated abnormally (e.g., signal)
            printf("\033[0;31m[CRASH]\033[0m Test process for '%s' terminated abnormally.\n", cmd);
            return 1; // Consider abnormal termination as an error
        }
    }
}

// ...existing code...

/**
 * Checks if a command has valid syntax according to minishell rules
 * Returns 1 if syntax error detected, 0 if syntax is valid
 */
int minishell_syntax_check(const char *cmd)
{
    int result;
    pid_t pid;
    int status;

    pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1; // Assume error in case fork fails
    }

    if (pid == 0) {
        // Child process: Run the syntax check
        t_lex_list *token = lexing_the_thing((char *)cmd);
        if (token == NULL && strlen(cmd) > 0 && strspn(cmd, " \t\n\v\f\r") != strlen(cmd)) {
            // Empty token list for non-whitespace command means lexing error
            exit(EXIT_FAILURE);
        }

        if (token == NULL) {
            // Empty or whitespace-only command is valid
            exit(EXIT_SUCCESS);
        }

        set_the_arg_type(token);
        
        // This will exit with error code if syntax error is found
        handle_syntax_errors(token);
        
        // If we get here, no syntax error was found
        // Free tokens if needed
        // free_lex_list(token);
        exit(EXIT_SUCCESS);
    } else {
        // Parent process: Wait for child and check status
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status)) {
            result = WEXITSTATUS(status) != 0;
        } else {
            // Abnormal termination, consider as error
            result = 1;
        }
        
        return result;
    }
}

/**
 * Checks if a command has valid syntax according to bash
 * Returns 1 if syntax error detected, 0 if syntax is valid
 */
int bash_syntax_check(const char *cmd)
{
    int fd;
    char tmp_file[] = "/tmp/bash_syntax_XXXXXX";
    int status;
    pid_t pid;
    int result;
    
    // Create temporary file for command
    fd = mkstemp(tmp_file);
    if (fd == -1) {
        perror("mkstemp");
        return 1; // Assume error in case of failure
    }
    
    // Write command to temp file
    if (write(fd, cmd, strlen(cmd)) == -1) {
        close(fd);
        unlink(tmp_file);
        return 1;
    }
    close(fd);
    
    pid = fork();
    if (pid == -1) {
        perror("fork");
        unlink(tmp_file);
        return 1;
    }
    
    if (pid == 0) {
        // Child process: redirect stderr to /dev/null to suppress bash error messages
        int null_fd = open("/dev/null", O_WRONLY);
        if (null_fd != -1) {
            dup2(null_fd, STDERR_FILENO);
            close(null_fd);
        }
        
        // Execute bash with -n flag to check syntax only
        execlp("bash", "bash", "-n", tmp_file, NULL);
        
        // If exec fails
        perror("execlp");
        exit(EXIT_FAILURE);
    } else {
        // Parent process: wait for child and check status
        waitpid(pid, &status, 0);
        
        // Remove temporary file
        unlink(tmp_file);
        
        if (WIFEXITED(status)) {
            // bash returns 0 for valid syntax, non-zero for errors
            result = WEXITSTATUS(status) != 0;
        } else {
            // Abnormal termination, consider as error
            result = 1;
        }
        
        return result;
    }
}

// No change needed to the main function since it's already using the functions
// we've now implemented

// ...existing code...

/**
 * Bash Compatibility Test Suite for Minishell
 * Tests syntax error detection against actual bash behavior
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <unistd.h>
 #include <sys/wait.h>
 
 // Colors for output formatting
 #define RED "\033[0;31m"
 #define GREEN "\033[0;32m"
 #define YELLOW "\033[0;33m"
 #define BLUE "\033[0;34m"
 #define RESET "\033[0m"
 
 /**
  * Tests if bash considers a command syntactically valid
  * Returns 0 for valid syntax, non-zero for syntax error
  */


 int main(void)
 {
	 // Array of test commands (approx 70)
	 const char *tests[] = {
		 // BASIC COMMANDS
		 "ls < > lopo <> ee",                               // Simple command
		 "ls -la",                                   // Command with options
		 "echo hello world",                         // Multiple arguments
		 "",                                         // Empty command
		 "/bin/ls",                                  // Path command
 
		 // PIPES
		 "ls | wc",                                  // Simple pipe
		 "ls -l | grep .c | wc -l",                 // Multiple pipes
		 "ls  |  wc",                               // Pipe with spaces
		 "ls |",                                    // Pipe at end (error)
		 "| ls",                                    // Pipe at beginning (error)
		 "ls | | wc",                               // Empty pipe (error)
		 "ls || wc",                                // Logical OR in bash, special in minishell
		 "ls ||| wc",                               // Triple pipe (error)
		 "cat | grep a | wc -l | sort | uniq",      // Long pipeline
		 "echo hello|wc -l",                        // No spaces around pipe
 
		 // REDIRECTIONS
		 "cat < file.txt",                          // Input redirection
		 "echo hello > out.txt",                    // Output redirection
		 "echo hello >> out.txt",                   // Append redirection
		 "cat << EOF",                              // Here document
		 "cat < in.txt > out.txt",                  // Multiple redirections
		 "cat <",                                   // Missing input target (error)
		 "echo >",                                  // Missing output target (error)
		 "cat <<",                                  // Missing heredoc delimiter (error)
		 "echo > > file.txt",                       // Double redirection operator (error)
		 "> out.txt",                               // Redirection without command (error)
		 "cat < file1.txt < file2.txt",             // Multiple input redirections
		 "echo hello > file1.txt > file2.txt",      // Multiple output redirections
		 "echo hello >> file1.txt >> file2.txt",    // Multiple append redirections
		 "< in.txt cat > out.txt",                  // Redirections surrounding command
		 "cat < \"file with spaces.txt\"",          // Redirection with quoted filename
 
		 // QUOTES
		 "echo \"hello world\"",                    // Double quotes
		 "echo 'hello world'",                      // Single quotes
		 "echo \"hello\" 'world'",                  // Mixed quotes
		 "echo \"hello \\\"world\\\"\"",            // Escaped quotes inside quotes
		 "echo \"$HOME\"",                          // Variable in quotes
		 "echo \"hello | world\"",                  // Special chars in quotes
		 "echo 'hello > world'",                    // Redirection in quotes
		 "echo \"hello\"world",                     // Adjacent quoted and unquoted
		 "\"echo\" hello",                          // Quoted command
		 "echo \"hello\"'world'",                   // Adjacent different quotes
 
		 // PIPES WITH REDIRECTIONS
		 "cat < file.txt | grep pattern",           // Input redirection with pipe
		 "grep pattern | sort > out.txt",           // Output redirection with pipe
		 "cat < in.txt | grep a > out.txt",         // Both redirections with pipe
		 "ls | < file.txt grep pattern",            // Input redirection after pipe
		 "cat file.txt | grep test > results.txt | wc -l", // Redirection between pipes
		 "ls | > file",                             // Pipe to redirection (error)
		 "ls > | wc",                               // Redirection to pipe (error)
		 "< in.txt cat | grep a | wc > out.txt",    // Complex pipeline with redirections
		 "echo hello | grep a > out.txt | wc -l",   // Pipe after redirection
		 "ls | wc < file.txt",                      // Redirection after pipe
 
		 // PARENTHESES (if supported)
		 "(ls)",                                    // Simple grouping
		 "(ls -l) | wc -l",                         // Grouping with pipe
		 "(ls) > file.txt",                         // Grouping with redirection
		 "( (ls) | wc )",                           // Nested grouping
		 "(ls",                                     // Unclosed parentheses (error)
 
		 // LOGICAL OPERATORS (assuming not supported in minishell)
		 "ls && echo hello",                        // Logical AND
		 "ls || echo hello",                        // Logical OR
		 "ls && echo hello || echo world",          // Mixed logical operators
		 "&& ls",                                   // Leading logical operator (error)
		 "ls &&",                                   // Trailing logical operator (error)
 
		 // EDGE CASES
		 ">",                                       // Just redirection operator
		 "|",                                       // Just pipe operator
		 "cat > \"out file.txt\"",                  // Quoted filename
		 "echo hello > file.txt world",             // Redirection in middle of command
		 "123",                                     // Number as command
		 "echo $HOME",                              // Environment variable
		 "echo hello\\>world",                      // Escaped special character
		 "echo hello #comment",                     // Command with comment 
		 "echo \"\\\"hello\\\"\"",                  // Quoted quotes
		 "ls | < file.txt grep pattern"             // Valid syntax, execution would fail
	 };
 
	 int test_count = sizeof(tests) / sizeof(tests[0]);
	 int match_count = 0;
 
	 printf("\n%s=== MINISHELL SYNTAX COMPATIBILITY TEST (%d tests) ===%s\n\n", BLUE, test_count, RESET);
	 printf("%-40s %-10s %-10s %-10s\n", "COMMAND", "BASH", "MINISHELL", "MATCH");
	 printf("%-40s %-10s %-10s %-10s\n", "----------------------------------------", "----------", "----------", "----------");
 
	 for (int i = 0; i < test_count; i++) {
		 int bash_result = bash_syntax_check(tests[i]);
		 int minishell_result = minishell_syntax_check(tests[i]);
		 
		 // Normalize results to 0 (valid) or 1 (error)
		 bash_result = bash_result ? 1 : 0;
		 minishell_result = minishell_result ? 1 : 0;
		 
		 int match = (bash_result == minishell_result);
		 if (match) {
			 match_count++;
		 }
		 
		 char cmd_display[41];
		 if (strlen(tests[i]) > 37) {
			 strncpy(cmd_display, tests[i], 37);
			 strcpy(cmd_display + 37, "...");
		 } else {
			 strcpy(cmd_display, tests[i]);
		 }
		 
		 printf("%-40s %-10s %-10s %s%s%s\n", 
			 cmd_display,
			 bash_result ? RED"ERROR"RESET : GREEN"VALID"RESET,
			 minishell_result ? RED"ERROR"RESET : GREEN"VALID"RESET,
			 match ? GREEN : RED,
			 match ? "YES" : "NO",
			 RESET
		 );
		 
		 if (!match) {
			 printf("  %s> Full command: '%s'%s\n", YELLOW, tests[i], RESET);
		 }
	 }
	 
	 float compatibility = (float)match_count / test_count * 100.0f;
	 
	 printf("\n%s=== TEST SUMMARY ===%s\n", BLUE, RESET);
	 printf("Total Tests: %d\n", test_count);
	 printf("Matches: %d/%d (%.1f%%)\n", match_count, test_count, compatibility);
	 
	 if (compatibility == 100.0) {
		 printf("\n%sCongratulations! Your minishell has perfect syntax compatibility with bash.%s\n\n", GREEN, RESET);
	 } else if (compatibility >= 90.0) {
		 printf("\n%sExcellent! Your minishell has very high syntax compatibility with bash.%s\n\n", GREEN, RESET);
	 } else if (compatibility >= 75.0) {
		 printf("\n%sGood! Your minishell has decent syntax compatibility with bash.%s\n\n", YELLOW, RESET);
	 } else {
		 printf("\n%sYour minishell needs more work to match bash syntax behavior.%s\n\n", RED, RESET);
	 }
	 
	 return (match_count == test_count) ? EXIT_SUCCESS : EXIT_FAILURE;
 }
 
 
 
 

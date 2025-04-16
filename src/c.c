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

// void	set_precedence_value(t_lex_list *token)
// {
// 	while (token)
// 	{
// 		if ()
// 		token = token->next;
// 	}
// }
int	is_special_opperand(int n)
{
	if (n != WORD)
		return (1);
	return (0);
}

int	is_special_for_pipe(int n)
{
	if (n != WORD && n != OP_PAREN)
		return (1);
	return (0);
}

int	is_special_for_specials(int n)
{
	if (n != WORD && n != OP_PAREN && n!= CL_PAREN)
		return (1);
	return (0);
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

void	handle_syntax_errors(t_lex_list *token)
{
	if (!token)
		return;
	if (check_valid_parenthesis(token) == 0)
		put_syntax_error();
	if (token->a_type == PIPE || token->a_type == AND || token->a_type == OR)
		put_syntax_error();
	if (token && is_special_opperand(token->a_type) && token->next
		&& is_special_opperand(token->next->a_type))
		put_syntax_error();
	while (token)
	{
		if (is_special_opperand(token) && token->next  && (token->next->a_type == HEREDOC || token->next->a_type == IN_REDIR))
			write(1,"",0);
		else if (is_special_for_specials(token->a_type) && token->next && is_special_for_specials(token->next->a_type))
			put_syntax_error();
		else if (token->a_type == PIPE && !token->next)
			put_syntax_error();
		else if (token->a_type == PIPE && token->next
			&& token->next->a_type != IN_REDIR && is_special_for_pipe(token->next->a_type))
			put_syntax_error();
		else if (token->a_type <= 3 && !token->next)
			put_syntax_error();
		else if (token->a_type <= 3 && token->next
			&& is_special_opperand(token->next->a_type))
			put_syntax_error();
		else if (token->a_type <= 7 && !token->next)
			put_syntax_error();

		token = token->next;
	}
}/**
 * Advanced Minishell Syntax Test Suite
 * 
 * This file contains structured tests for your minishell syntax checker.
 * Now includes tests for unclosed quotes and more complex command structures.
 */
/**
 * Comprehensive Minishell Syntax Test Suite
 *
 * This file contains 150 structured tests for your minishell syntax checker.
 * Each test is designed to thoroughly evaluate your syntax error detection.
 */

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <sys/wait.h>
// #include <sys/types.h>


// // Assume minishell.h defines t_lex_list and the necessary functions:
// // typedef struct s_lex_list { ... } t_lex_list;
// // t_lex_list *lexing_the_thing(char *cmd);
// // void set_the_arg_type(t_lex_list *token);
// // void handle_syntax_errors(t_lex_list *token); // Exits on error

// typedef struct s_test {
//     char    *name;
//     char    *cmd;
//     int     expected; // 0 for valid syntax, 1 for syntax error (based on run_syntax_test logic)
//     char    *description;
// } t_test;

// // Function to print test results with color
// void print_test_result(char *name, int result, int expected)
// {
//     if (result == expected)
//         printf("\033[0;32m[PASS]\033[0m %s\n", name);
//     else
//         printf("\033[0;31m[FAIL]\033[0m %s (got %d, expected %d)\n",
//                name, result, expected);
// }

// // Function to run the syntax check in a separate process
// // Returns 0 if syntax is valid (child exits 0)
// // Returns 1 if syntax is invalid (child exits non-zero or terminates abnormally)
// int run_syntax_test(char *cmd)
// {
//     pid_t pid;
//     int status;

//     pid = fork();
//     if (pid == -1) {
//         perror("fork");
//         exit(EXIT_FAILURE); // Exit test suite if fork fails
//     }

//     if (pid == 0) {
//         // Child process: Run the syntax check
//         // Redirect stderr to /dev/null to suppress minishell error messages during tests
//         freopen("/dev/null", "w", stderr);

//         t_lex_list *token = lexing_the_thing(cmd);
//         if (token == NULL && strlen(cmd) > 0 && strspn(cmd, " \t\n\v\f\r") != strlen(cmd)) {
//             // If lexing fails for non-empty/non-whitespace command, consider it a failure
//             // Or handle lexing errors explicitly if lexing_the_thing indicates them
//             exit(EXIT_FAILURE); // Indicate lexing error
//         }

//         // Handle empty/whitespace commands gracefully (usually valid syntax)
//         if (token == NULL) {
//              exit(EXIT_SUCCESS); // Empty/whitespace line is syntactically valid
//         }

//         set_the_arg_type(token);
//         handle_syntax_errors(token); // This function should exit(2) or similar on syntax error

//         // If handle_syntax_errors returns or doesn't exit, syntax is considered valid
//         // Make sure to free tokens if handle_syntax_errors doesn't free on success
//         // free_lex_list(token); // Add your token freeing function here if needed
//         exit(EXIT_SUCCESS); // No syntax error detected

//     } else {
//         // Parent process: Wait for the child and check status
//         waitpid(pid, &status, 0);

//         if (WIFEXITED(status)) {
//             // Child exited normally
//             // WEXITSTATUS returns the exit code of the child
//             // We expect 0 for success (valid syntax) and non-zero for error (invalid syntax)
//             return WEXITSTATUS(status) != 0; // Return 1 if exit status is non-zero (error), 0 otherwise
//         } else {
//             // Process terminated abnormally (e.g., signal)
//             printf("\033[0;31m[CRASH]\033[0m Test process for '%s' terminated abnormally.\n", cmd);
//             return 1; // Consider abnormal termination as an error
//         }
//     }
// }

// int main(void)
// {
//     // Array of test cases - 150 comprehensive tests
//     t_test tests[] = {
//         // ===== BASIC COMMANDS =====
//         { "Basic command", "echo hello", 0, "Basic command with one argument" },
//         { "Multi-word command", "echo hello world", 0, "Basic command with multiple arguments" },
//         { "Command with options", "ls -la", 0, "Command with dash options" },
//         { "Long command", "echo this is a very long command with many arguments to test the parser", 0, "Long command with many arguments" },
//         { "Command with special chars", "echo hello! world? test.", 0, "Command with special characters in arguments" },
//         { "Empty command", "", 0, "Empty command line" },
//         { "Spaces only", "      ", 0, "Command line with only spaces" },
//         { "Tabs only", "\t\t\t", 0, "Command line with only tabs" },
//         { "Mixed whitespace", " \t \t ", 0, "Command line with mixed whitespace" },
//         { "Binary path", "/bin/ls", 0, "Command with full path" },

//         // ===== PIPE TESTS =====
//         { "Pipe Simple", "echo test | grep t", 0, "Command with a single pipe" },
//         { "Pipe Multiple", "ls -la | grep .c | wc -l", 0, "Command with multiple pipes" },
//         { "Pipe with whitespace", "echo test  |  grep t", 0, "Pipe with extra whitespace around it" },
//         { "Pipe Empty end", "echo test |", 1, "Command with pipe at the end (syntax error)" },
//         { "Pipe Empty start", "| echo test", 1, "Command starting with pipe (syntax error)" },
//         { "Pipe Empty middle", "echo test | | wc", 1, "Command with empty pipe in the middle (syntax error)" },
//         { "Pipe Multiple consecutive", "echo test ||| grep t", 1, "Multiple consecutive pipes (syntax error)" },
//         { "Pipe Double logical OR", "echo test || grep t", 1, "Double pipe (syntax error in minishell context, might be OR in bash)" }, // Assuming || is not supported as pipe
//         { "Pipe Many commands", "ls | grep .c | wc -l | cat | sort | uniq | head", 0, "Many commands connected with pipes" },
//         { "Pipe Chain with empty commands", "ls | | | grep .c", 1, "Multiple empty commands in pipe chain (syntax error)" },

//         // ===== QUOTE TESTS =====
//         { "Quote Double", "echo \"hello world\"", 0, "Command with double quotes" },
//         { "Quote Single", "echo 'hello world'", 0, "Command with single quotes" },
//         { "Quote Empty", "echo \"\" ''", 0, "Command with empty quotes" },
//         { "Quote Mixed", "echo \"hello 'world'\"", 0, "Command with nested quotes" },
//         { "Quote Multiple parts", "echo \"hello\"'world'\"how\"'are'\"you\"", 0, "Command with multiple quoted parts concatenated" },
//         { "Quote with spaces", "echo \"   hello   world   \"", 0, "Quoted text with internal spaces" },
//         { "Quote Special chars", "echo \"hello | world > file\"", 0, "Special chars inside quotes (should be treated as literal)" },
//         { "Quote Unclosed double", "echo \"hello world", 1, "Command with unclosed double quotes (syntax error)" },
//         { "Quote Unclosed single", "echo 'hello world", 1, "Command with unclosed single quotes (syntax error)" },
//         { "Quote Multiple unclosed", "echo \"hello 'world", 1, "Command with multiple unclosed quotes (syntax error)" },
//         { "Quote Start unclosed", "\"echo hello\"", 1, "Command starts with unclosed quote" },
//         { "Quote End unclosed", "echo hello\"", 1, "Command ends with unclosed quote" },

//         // ===== REDIRECTION TESTS =====
//         { "Redirection Input", "cat < file.txt", 0, "Simple input redirection" },
//         { "Redirection Output", "echo hello > output.txt", 0, "Simple output redirection" },
//         { "Redirection Append", "echo hello >> output.txt", 0, "Append output redirection" },
//         { "Redirection Heredoc", "cat << EOF", 0, "Heredoc redirection (syntax only)" }, // Assuming << is supported
//         { "Redirection Mixed", "cat < input.txt > output.txt", 0, "Command with both input and output redirections" },
//         { "Redirection Multiple Input", "cat < input1.txt < input2.txt", 0, "Multiple input redirections (last one usually takes effect)" },
//         { "Redirection Multiple Output", "echo hello > file1.txt > file2.txt", 0, "Multiple output redirections (behavior depends on implementation, syntax is usually ok)" },
//         { "Redirection Multiple Append", "echo hello >> file1.txt >> file2.txt", 0, "Multiple append redirections" },
//         { "Redirection Multiple Heredoc", "cat << EOF1 << EOF2", 0, "Multiple heredocs (last one usually takes effect)" },
//         { "Redirection with spaces", "cat < input.txt   >   output.txt", 0, "Redirections with extra whitespace" },
//         { "Redirection with quotes", "cat < \"input file.txt\" > 'output file.txt'", 0, "Redirections with quoted filenames" },
//         { "Redirection Missing output target", "echo hello >", 1, "Output redirection without target (syntax error)" },
//         { "Redirection Missing input target", "cat <", 1, "Input redirection without target (syntax error)" },
//         { "Redirection Missing append target", "echo hello >>", 1, "Append redirection without target (syntax error)" },
//         { "Redirection Missing heredoc target", "cat <<", 1, "Heredoc redirection without delimiter (syntax error)" },

//         // ===== ADVANCED REDIRECTION TESTS =====
//         { "Advanced Redir Missing target with space", "echo hello > ", 1, "Output redirection with space but no target (syntax error)" },
//         { "Advanced Redir Multiple operators >> >", "echo hello >> > output.txt", 1, "Multiple consecutive mixed redirection operators (syntax error)" },
//         { "Advanced Redir Multiple operators > >>", "echo hello > >> output.txt", 1, "Multiple consecutive mixed redirection operators (syntax error)" },
//         { "Advanced Redir Multiple operators < <", "cat < < input.txt", 1, "Multiple consecutive input redirection operators (syntax error)" },
//         { "Advanced Redir Multiple operators << <<", "cat << << EOF", 1, "Multiple consecutive heredoc operators (syntax error)" },
//         { "Advanced Redir Multiple operators > >", "echo hello > > output.txt", 1, "Multiple consecutive output redirection operators (syntax error)" },
//         { "Advanced Redir Multiple operators >> >>", "echo hello >> >> output.txt", 1, "Multiple consecutive append redirection operators (syntax error)" },
//         { "Advanced Redir to pipe", "echo hello > | grep hello", 1, "Redirection directly to pipe (syntax error)" },
//         { "Advanced Redir Pipe to redirection without command", "echo hello | > file.txt", 1, "Pipe to redirection without command (syntax error)" },
//         { "Advanced Redir Chain", "echo hello > file1.txt > file2.txt > file3.txt", 0, "Chain of output redirections" },
//         { "Advanced Redir between arguments", "echo hello > file.txt world", 0, "Command with redirections between arguments" },
//         { "Advanced Redir before command", "< input.txt cat", 0, "Redirection before command name" },
//         { "Advanced Redir with special chars filename", "echo hello > file-name_123.txt", 0, "Redirection with special characters in filename" },
//         { "Advanced Redir Mixed pipes", "cat < input.txt | grep pattern > output.txt", 0, "Combination of redirections and pipes" },
//         { "Advanced Redir Complex sequence", "cat < input.txt > output1.txt | grep pattern >> output2.txt", 0, "Complex sequence of redirections and pipes" },
//         { "Advanced Redir No command >", "> output.txt", 1, "Output redirection without command (syntax error)" },
//         { "Advanced Redir No command <", "< input.txt", 1, "Input redirection without command (syntax error)" },
//         { "Advanced Redir No command >>", ">> output.txt", 1, "Append redirection without command (syntax error)" },
//         { "Advanced Redir No command <<", "<< EOF", 1, "Heredoc redirection without command (syntax error)" },
//         { "Advanced Redir Pipe ends with redir", "ls | > ", 1, "Pipe ends with redirection operator (syntax error)"},
//         { "Advanced Redir Pipe starts with redir", "> file | wc", 1, "Pipe starts with redirection operator (syntax error)"},

//         // ===== COMBINED QUOTE AND REDIRECTION TESTS =====
//         { "Combined Quoted redir target", "echo hello > \"output file.txt\"", 0, "Redirection to quoted filename" },
//         { "Combined Quoted redir operator >", "echo \"hello > world\"", 0, "Quoted redirection operator > (not a redirection)" },
//         { "Combined Quoted redir operator <", "echo \"hello < world\"", 0, "Quoted redirection operator < (not a redirection)" },
//         { "Combined Quoted redir operator >>", "echo \"hello >> world\"", 0, "Quoted redirection operator >> (not a redirection)" },
//         { "Combined Quoted redir operator <<", "echo \"hello << world\"", 0, "Quoted redirection operator << (not a redirection)" },
//         { "Combined Quoted pipe", "echo \"hello | world\"", 0, "Quoted pipe character (not a pipe)" },
//         { "Combined Mixed quotes and redir", "echo \"hello > 'world'\" > output.txt", 0, "Mixed quotes with actual redirection" },
//         { "Combined Redir in single quotes", "echo 'hello > world'", 0, "Redirection in single quotes (not a redirection)" },
//         { "Combined Unclosed quotes with redir", "echo \"hello > output.txt", 1, "Unclosed quotes with redirection (syntax error)" },
//         { "Combined Unclosed quotes spanning redir", "echo \"hello > output.txt | grep pattern", 1, "Unclosed quotes spanning redirection and pipe (syntax error)" },
//         { "Combined Quoted filename unclosed", "echo hello > \"file with spaces", 1, "Redirection to quoted filename with unclosed quotes (syntax error)" },
//         { "Combined Redir between quoted text", "echo \"hello\" > file.txt \"world\"", 0, "Redirection between quoted text segments" },
//         { "Combined Quoted text multiple redirs", "echo \"hello world\" > file1.txt >> file2.txt", 0, "Quoted text with multiple redirections" },
//         { "Combined Quoted redir op followed by real", "echo \">\" > file.txt", 0, "Quoted operator followed by real redirection"},

//         // ===== COMBINED PIPE AND QUOTE TESTS =====
//         { "Combined Pipes with quoted commands", "echo \"hello world\" | grep \"world\"", 0, "Pipes with quoted commands" },
//         { "Combined Unclosed quotes crossing pipe", "echo \"hello | grep world", 1, "Unclosed quotes that cross a pipe (syntax error)" },
//         { "Combined Multiple quoted pipe segments", "echo \"hello\" | grep 'world' | wc -l", 0, "Multiple pipe segments with quotes" },
//         { "Combined Quotes containing pipe", "echo \"This | is not a pipe\" | grep This", 0, "Quoted text containing pipe character" },
//         { "Combined Quotes after pipe", "echo hello | grep \"world pattern\"", 0, "Quotes in command after pipe" },
//         { "Combined Quotes before pipe", "echo \"hello pattern\" | grep pattern", 0, "Quotes in command before pipe" },
//         { "Combined Complex quotes and pipes", "echo \"hello | not a pipe\" | grep 'pattern | also not a pipe'", 0, "Complex quotes containing pipe characters" },
//         { "Combined Single quotes with pipes", "echo 'hello | world' | grep 'hello'", 0, "Single quotes with pipes" },
//         { "Combined Alternating quotes across pipes", "echo \"hello\"'world' | grep 'pattern'\"search\"", 0, "Alternating quote types across pipes" },
//         { "Combined Nested quotes across pipes", "echo \"hello 'world'\" | grep 'pattern \"search\"'", 0, "Nested quotes across pipes" },
//         { "Combined Pipe next to quote start", "echo \"hello\"|grep world", 0, "Pipe immediately after closing quote"},
//         { "Combined Pipe next to quote end", "echo hello|\"grep world\"", 0, "Pipe immediately before opening quote"},

//         // ===== COMPLEX COMMAND COMBINATIONS =====
//         // Note: Some tests previously marked as "Complex error" might be syntactically valid depending on shell specifics
//         { "Complex Combo 1", "echo test | grep test > file.txt | wc -l", 0, "Command with pipes and redirections" }, // This is ambiguous in bash, redirection might apply to echo or wc. Assume applies to grep for syntax check.
//         { "Complex Combo 2", "cat < \"file with spaces.txt\" | sort | uniq > \"output file.txt\"", 0, "Redirections, pipes, quoted filenames" },
//         { "Complex Combo 3", "echo \"text with a | pipe\" > file.txt | grep text", 0, "Quoted pipe character and actual pipe" },
//         { "Complex Combo 4", "cat file.txt | grep \"search term\" | sed 's/old/new/g' > output.txt", 0, "Multiple pipes with quotes and redirection" },
//         { "Complex Combo 5", "ls -la | grep \".c$\" | sort -r | head -n 5 > top_files.txt", 0, "Complex pipeline with multiple commands and redirection" },
//         { "Complex Combo 6", "< input.txt cat | grep pattern | sort | uniq -c | sort -nr | head -n 10 > output.txt", 0, "Long pipeline with input and output redirection" },
//         //{ "Complex command 7", "echo \"hello world\" > file1.txt ; cat file1.txt | grep hello", 0, "Command sequence with semicolon" }, // Semicolon tests below
//         { "Complex Combo 8", "echo hello > file1.txt >> file2.txt | cat file1.txt file2.txt", 0, "Multiple redirections before pipe" }, // Ambiguous redirection target, syntax might be ok
//         { "Complex Combo 9", "cat < input1.txt < input2.txt | grep pattern > output.txt", 0, "Multiple input redirections with pipe and output redirection" },
//         { "Complex Combo 10", "echo \"text with > redirect\" | grep text > output.txt", 0, "Quoted redirect character and actual redirect" },
//         { "Complex Combo 11 Pipe Redir Mix", "<infile grep word | wc -l > outfile", 0, "Input redir, pipe, output redir"},
//         { "Complex Combo 12 Redir Order", "> outfile wc -l < infile", 0, "Redirections surrounding command"},

//         // ===== COMPLEX ERROR CASES =====
//         { "Complex Error Unclosed quote pipe", "echo \"unclosed quote | grep test", 1, "Unclosed quote spanning a pipe (syntax error)" },
//         { "Complex Error Empty pipe complex", "echo test > file.txt | | grep pattern", 1, "Empty pipe in a complex command (syntax error)" },
//         { "Complex Error Consecutive input redir < <", "cat < < input.txt", 1, "Consecutive input redirections (syntax error)" },
//         { "Complex Error Consecutive output redir > >", "echo test > > output.txt", 1, "Consecutive output redirections (syntax error)" },
//         { "Complex Error Pipe at end complex", "echo test | grep pattern |", 1, "Pipe at end of complex command (syntax error)" },
//         { "Complex Error Redir no target in pipe", "echo test | grep pattern > | wc", 1, "Redirection without target in middle of pipe (syntax error)" },
//         // { "Complex error 7", "echo test | < input.txt grep pattern", 0, "Input redirection after pipe (valid but complex)" }, // Test 134 below covers this better
//         { "Complex Error Input redir no src pipe", "< | cat", 1, "Input redirection without source followed by pipe (syntax error)" },
//         { "Complex Error Output redir no cmd", "> output.txt", 1, "Output redirection without command (syntax error)" },
//         { "Complex Error Redir between pipes no cmd", "echo test | > output.txt | wc", 1, "Output redirection between pipes without command (syntax error)" },
//         { "Complex Error Triple Pipe", "ls ||| wc", 1, "Triple pipe operator"},
//         { "Complex Error Pipe then Redir Operator", "ls | > file", 1, "Pipe followed directly by redirection operator"},
//         { "Complex Error Redir Operator then Pipe", "ls > | wc", 1, "Redirection operator followed directly by pipe"},
//         { "Complex Error Mixed Operators Pipe Redir", "ls | < file", 1, "Pipe followed by input redirection operator"}, // This is technically valid in bash (error redirection) but likely syntax error for minishell
//         { "Complex Error Mixed Operators Redir Pipe", "ls < | wc", 1, "Input redirection operator followed by pipe"},

//         // ===== EDGE CASES =====
//         // Escaping might be handled by lexer or parser. Assuming basic syntax validity check.
//         { "Edge Backslash Space", "echo hello\\ world", 0, "Command with escaped space (valid if lexer handles)" },
//         { "Edge Escaped Double Quote", "echo \\\"hello\\\"", 0, "Command with escaped double quotes (valid if lexer handles)" },
//         { "Edge Escaped Single Quote", "echo \\'hello\\'", 0, "Command with escaped single quotes (valid if lexer handles)" },
//         { "Edge Escaped Pipe", "echo hello\\|world", 0, "Command with escaped pipe (valid if lexer handles)" },
//         { "Edge Escaped Redirection", "echo hello\\>world", 0, "Command with escaped redirection (valid if lexer handles)" },
//         { "Edge Escape at end", "echo hello\\", 0, "Command with escape at end of line (often means line continuation or literal backslash)" },
//         //{ "Edge Escaped newline", "echo hello\\\nworld", 0, "Command with escaped newline (if your parser supports it)" }, // Hard to test reliably here
//         { "Edge Quotes escaped quote", "echo \"hello\\\"world\"", 0, "Double quotes with internal escaped quote (valid if lexer handles)" },
//         { "Edge Single Quotes escaped quote", "echo 'hello\\'world'", 0, "Single quotes with internal escaped quote (backslash usually literal in single quotes)" },
//         { "Edge Single char command", "a", 0, "Single character command" },
//         { "Edge Env var", "echo $HOME", 0, "Command with environment variable (syntax check only)" },
//         { "Edge Tilde", "ls ~", 0, "Command with tilde expansion (syntax check only)" },
//         { "Edge Tilde path", "ls ~/Documents", 0, "Command with tilde path (syntax check only)" },
//         { "Edge Empty string arg", "grep \"\" file.txt", 0, "Command with empty string argument"},
//         { "Edge Number command", "123", 0, "Command is just a number (might be error at execution, syntax ok)"},
//         { "Edge Redir only", ">", 1, "Just a redirection operator" },
//         { "Edge Pipe only", "|", 1, "Just a pipe operator" },

//         // ===== HEREDOC TESTS (Syntax only - requires << support) =====
//         { "Heredoc Basic", "cat << EOF", 0, "Basic heredoc syntax" },
//         { "Heredoc Quoted Delimiter", "cat << \"EOF\"", 0, "Heredoc with quoted delimiter" },
//         { "Heredoc Single Quoted Delimiter", "cat << 'EOF'", 0, "Heredoc with single quoted delimiter" },
//         { "Heredoc With Pipe", "cat << EOF | grep pattern", 0, "Heredoc followed by pipe" },
//         { "Heredoc Multiple", "cat << EOF1 << EOF2", 0, "Multiple heredocs" }, // Last one active
//         { "Heredoc With Output Redir", "cat << EOF > output.txt", 0, "Heredoc with output redirection" },
//         { "Heredoc Missing Delimiter", "cat <<", 1, "Heredoc missing delimiter" },
//         { "Heredoc Before Command", "<< EOF cat", 0, "Heredoc redirection before command"},
//         { "Heredoc Mixed Redir", "cat << EOF < file.txt", 0, "Heredoc mixed with input redirection"},
//         { "Heredoc Empty Delimiter", "cat << \"\"", 0, "Heredoc with empty quoted delimiter"}, // Bash allows this, might be edge case

//         // ===== LOGIC OPERATOR TESTS (IF SUPPORTED) =====
//         // Assuming minishell does NOT support && and || syntax initially, so mark as errors (expected=1)
//         // Change expected to 0 if your minishell *does* support them.
//         { "Logic AND", "echo hello && echo world", 1, "Commands with AND operator (mark as error if unsupported)" },
//         { "Logic OR", "false || echo world", 1, "Commands with OR operator (mark as error if unsupported)" },
//         { "Logic Mixed", "echo hello && echo world || echo test", 1, "Commands with mixed logic operators (mark as error if unsupported)" },
//         { "Logic With Pipe", "echo hello | grep hello && echo found", 1, "Logic operators with pipe (mark as error if unsupported)" },
//         { "Logic With Redirection", "echo hello > file.txt && cat file.txt", 1, "Logic operators with redirection (mark as error if unsupported)" },
//         { "Logic Empty AND Left", "&& echo hello", 1, "AND operator at start (syntax error)"},
//         { "Logic Empty AND Right", "echo hello &&", 1, "AND operator at end (syntax error)"},
//         { "Logic Empty OR Left", "|| echo hello", 1, "OR operator at start (syntax error)"},
//         { "Logic Empty OR Right", "echo hello ||", 1, "OR operator at end (syntax error)"},
//         { "Logic Multiple AND", "echo a && && echo b", 1, "Multiple consecutive AND operators (syntax error)"},


//         // ===== MISCELLANEOUS TESTS =====
//         { "Misc Numbers", "echo 123 456", 0, "Command with numeric arguments" },
//         { "Misc Special Chars Allowed", "echo arg-uments_are=okay+here./~", 0, "Command with generally allowed special characters in arguments"},
//         { "Misc Special Shell Chars", "echo ! # $ % & * ( ) + , - . / : ; = ? @ [ ] ^ _ ` { } ~", 0, "Command with many special characters (syntax check only, expansion/quoting matters)" }, // Some of these ARE syntax errors if unquoted (like ';', '&', '(', ')')
//         { "Misc Very Long", "echo this is a very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very long command", 0, "Very long command line (syntax check)" },
//         { "Misc Unicode", "echo 你好 世界", 0, "Command with Unicode characters (if terminal/lexer support UTF-8)" },
//         { "Misc Mixed Case", "EcHo HeLlO WoRlD", 0, "Command with mixed case (execution depends on finding 'EcHo')" },
//         { "Misc Comment", "# This is a comment", 0, "Comment line (should be ignored, valid syntax)"},
//         { "Misc Command After Comment", "echo hello # comment here", 0, "Command followed by comment"},
//         { "Misc Pathological Quotes", "echo ''\"\"''\"\"''", 0, "Consecutive empty quotes"},
//         { "Misc Mixed Operators No Space", "cat<file|wc>out", 0, "Operators without surrounding spaces"}, // Valid syntax
//         { "Misc Input Redir After Pipe", "ls | < file.txt wc", 0, "Input redirection specified after pipe for second command"}, // Valid syntax

//         // Add more tests as needed, especially for features like command substitution $(), grouping (), background &, etc. if implemented.
//         // Ensure the total number of tests matches the comment (or update comment). This list has > 150.
//     };

//     int test_count = sizeof(tests) / sizeof(t_test);
//     int passed = 0;
//     int category_passed = 0;
//     int category_count = 0;
//     char current_category[100] = "";

//     printf("\n===== COMPREHENSIVE MINISHELL SYNTAX TEST SUITE (%d tests) =====\n\n", test_count);

//     for (int i = 0; i < test_count; i++)
//     {
//         // Simplified category detection based on keywords in the test name
//         // Extracts the first word before a space as a potential category keyword
//         char test_category_keyword[100];
//         sscanf(tests[i].name, "%99s", test_category_keyword);

//         // Map keyword to a display name (can be more sophisticated)
//         char potential_new_category[100] = "";
//         if (strcmp(test_category_keyword, "Basic") == 0) strcpy(potential_new_category, "BASIC COMMANDS");
//         else if (strcmp(test_category_keyword, "Pipe") == 0) strcpy(potential_new_category, "PIPE TESTS");
//         else if (strcmp(test_category_keyword, "Quote") == 0) strcpy(potential_new_category, "QUOTE TESTS");
//         else if (strcmp(test_category_keyword, "Redirection") == 0) strcpy(potential_new_category, "REDIRECTION TESTS");
//         else if (strcmp(test_category_keyword, "Advanced") == 0) strcpy(potential_new_category, "ADVANCED REDIRECTION TESTS"); // Group advanced redir
//         else if (strcmp(test_category_keyword, "Combined") == 0) strcpy(potential_new_category, "COMBINED SYNTAX TESTS"); // Group combined tests
//         else if (strcmp(test_category_keyword, "Complex") == 0) strcpy(potential_new_category, "COMPLEX TESTS"); // Group complex tests
//         else if (strcmp(test_category_keyword, "Edge") == 0) strcpy(potential_new_category, "EDGE CASES");
//         else if (strcmp(test_category_keyword, "Heredoc") == 0) strcpy(potential_new_category, "HEREDOC TESTS");
//         else if (strcmp(test_category_keyword, "Logic") == 0) strcpy(potential_new_category, "LOGIC OPERATOR TESTS (if supported)");
//         else if (strcmp(test_category_keyword, "Semicolon") == 0) strcpy(potential_new_category, "SEMICOLON TESTS (if supported)");
//         else if (strcmp(test_category_keyword, "Misc") == 0) strcpy(potential_new_category, "MISCELLANEOUS TESTS");
//         // Add other categories explicitly if needed

//         // Check if the detected category is different from the current one
//         if (strlen(potential_new_category) > 0 && strcmp(current_category, potential_new_category) != 0)
//         {
//             // Print summary for the previous category if it existed and had tests
//             if (i > 0 && category_count > 0) {
//                 printf("\n----- %s Category Summary: %d/%d passed (%.1f%%) -----\n\n",
//                        current_category, category_passed, category_count,
//                        (category_count > 0) ? ((float)category_passed / category_count * 100.0) : 0.0);
//             }
//             // Update to the new category and reset counters
//             strcpy(current_category, potential_new_category);
//             category_passed = 0;
//             category_count = 0;
//             printf("===== %s =====\n", current_category);
//         } else if (i == 0 && strlen(potential_new_category) > 0) {
//              // Handle the very first category if detected
//             strcpy(current_category, potential_new_category);
//             printf("===== %s =====\n", current_category);
//         } else if (i == 0) {
//             // Default category if first test doesn't match known keywords
//             strcpy(current_category, "INITIAL TESTS");
//              printf("===== %s =====\n", current_category);
//         }


//         // Run the actual test
//         // printf("Testing: %s\n CMD: [%s]\n", tests[i].name, tests[i].cmd); // Debug print
//         int result = run_syntax_test(tests[i].cmd);
//         print_test_result(tests[i].name, result, tests[i].expected);

//         // Update counters
//         category_count++;
//         if (result == tests[i].expected) {
//             passed++;
//             category_passed++;
//         } else {
//             // Print details on failure for easier debugging
//             printf("  Failed Command: '%s'\n", tests[i].cmd);
//             printf("  Description: %s\n", tests[i].description);
//         }
//     }

//     // Print summary for the very last category
//     if (category_count > 0) {
//         printf("\n----- %s Category Summary: %d/%d passed (%.1f%%) -----\n\n",
//                current_category, category_passed, category_count,
//                (category_count > 0) ? ((float)category_passed / category_count * 100.0) : 0.0);
//     }

//     // Print overall summary
//     printf("======================================================\n");
//     printf("Overall Test Summary: %d / %d tests passed (%.1f%%)\n",
//            passed, test_count, (test_count > 0) ? ((float)passed / test_count * 100.0) : 0.0);
//     printf("======================================================\n");

//     // Return status indicating success (0) or failure (1)
//     return (passed == test_count) ? EXIT_SUCCESS : EXIT_FAILURE;
// }



int main()
{
	t_lex_list *token;
	token = lexing_the_thing("echo hello > file.txt && cat file.txt");
	set_the_arg_type(token);
	handle_syntax_errors(token);
	while (token)
	{
		printf("Token: /%s\\ Type: %d\n", token->s, token->a_type);
		token = token->next;
	}
}
# include "../includes/minishell_new.h"

void	put_syntax_error(void)
{
	t_lex_list	*p;

	p->status = 258;
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
	t_lex_list	*p;
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
		else if (str[i] == '(' || str[i] == ')')
		{
			quote = NQ;
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
	if (n == WORD || n == OP_PAREN || n == IN_REDIR || n == OUT_REDIR
		|| n == APPEND || n == HEREDOC)
		return (0);
	return (1);
}

int	is_special_for_specials(int n)
{
	// Also allow redirection operators in the special contexts
	if (n == WORD || n == OP_PAREN || n == CL_PAREN || n == IN_REDIR
		|| n == OUT_REDIR || n == APPEND || n == HEREDOC)
		return (0);
	return (1);
	// Also allow redirection operators in the special contexts
	if (n == WORD || n == OP_PAREN || n == CL_PAREN || n == IN_REDIR
		|| n == OUT_REDIR || n == APPEND || n == HEREDOC)
		return (0);
	return (1);
}

int	check_valid_parenthesis(t_lex_list *token)
{
	int	f;

	f = 0;
	while (token)
	{
		if (token->a_type == OP_PAREN)
			f++;
		else if (token->a_type == CL_PAREN)
			f--;
		if (f < 0)
			return (0);
		token = token->next;
	}
	if (f == 0)
		return (1);
	return (0);
}

void	ft_putstr_fd(int fd, char *s)
{
	if (s)
		write(fd, s, ft_strlen(s));
}
void	put_syntax_error_token(t_lex_list *token)
{
	char	*token_str;

	if (!token)
	{
		ft_putstr_fd(STDERR_FILENO,
			"minishell: syntax error near unexpected token `newline'\n");
	}
	else
	{
		if (token->s)
		{
			token_str = token->s;
		}
		else
		{
			token_str = "";
		}
		if (token->a_type == AND)
		{
			ft_putstr_fd(STDERR_FILENO,
				"minishell: syntax error near unexpected token `&&'\n");
		}
		else if (token->a_type == OR)
		{
			ft_putstr_fd(STDERR_FILENO,
				"minishell: syntax error near unexpected token `||'\n");
		}
		else if (token->a_type == PIPE)
		{
			ft_putstr_fd(STDERR_FILENO,
				"minishell: syntax error near unexpected token `|'\n");
		}
		else if (token->a_type == OUT_REDIR || token->a_type == APPEND
			|| token->a_type == IN_REDIR || token->a_type == HEREDOC)
		{
			ft_putstr_fd(STDERR_FILENO,
				"minishell: syntax error near unexpected token `");
			ft_putstr_fd(STDERR_FILENO, token_str);
			ft_putstr_fd(STDERR_FILENO, "'\n");
		}
		else
		{
			ft_putstr_fd(STDERR_FILENO,
				"minishell: syntax error near unexpected token `");
			ft_putstr_fd(STDERR_FILENO, token_str);
			ft_putstr_fd(STDERR_FILENO, "'\n");
		}
	}
	// change exit with return status later
	exit(258);
}

void	handle_syntax_errors(t_lex_list *token)
{
	t_lex_list	*current;
	t_lex_list	*next;

	if (!token)
		return ;
	current = token;
	if (current->a_type == PIPE || current->a_type == AND
		|| current->a_type == OR)
		return (put_syntax_error_token(current));
	if (check_valid_parenthesis(token) == 0)
		return (put_syntax_error_token(NULL));
	while (current)
	{
		next = current->next;
		if ((current->a_type == OUT_REDIR || current->a_type == APPEND
				|| current->a_type == IN_REDIR || current->a_type == HEREDOC)
			&& (!next || (next->a_type != WORD && next->a_type != OP_PAREN)))
		{
			if (next)
				return (put_syntax_error_token(next));
			else
				return (put_syntax_error_token(NULL));
		}
		if (current->a_type == PIPE && !next)
			return (put_syntax_error_token(NULL));
		if (current->a_type == PIPE && next && next->a_type == PIPE)
			return (put_syntax_error_token(next));
		if ((current->a_type == AND || current->a_type == OR) && (!next
				|| next->a_type == PIPE || next->a_type == AND
				|| next->a_type == OR))
		{
			if (next)
				return (put_syntax_error_token(next));
			else
				return (put_syntax_error_token(NULL));
		}
		current = next;
	}
}

void	print_tabs(int depth)
{
	while (depth--)
		write(1, "   ", 3);
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to create a clear visual separator in output
void print_separator(void)
{
    printf("\n=================================================\n");
    printf("=================================================\n");
}

// Improved tree printing function with depth tracking
void print_tree_detailed(t_ast_tree *node, int depth)
{
    if (!node)
        return;
    
    print_tabs(depth);
    printf("Node type: %d, Content: \"%s\"\n", node->type, node->content ? node->content : "NULL");
    
    if (node->args) {
        print_tabs(depth);
        printf("Arguments: ");
        for (int i = 0; node->args[i]; i++) {
            printf("\"%s\" ", node->args[i]);
        }
        printf("\n");
    }
    
    if (node->redirect) {
        print_tabs(depth);
        printf("Redirections:\n");
        t_redirect *temp = node->redirect;
        while (temp) {
            print_tabs(depth + 1);
            printf("Type: %d, File: \"%s\"\n", temp->type, temp->file_name);
            temp = temp->next;
        }
    }
    
    if (node->left) {
        print_tabs(depth);
        printf("Left child:\n");
        print_tree_detailed(node->left, depth + 1);
    }
    
    if (node->right) {
        print_tabs(depth);
        printf("Right child:\n");
        print_tree_detailed(node->right, depth + 1);
    }
}

// Function to test the parsing of a command
void test_command(char *cmd)
{
    printf("MEGA TEST: \"%s\"\n", cmd);
    
    t_lex_list *tokens = lexing_the_thing(cmd);
    if (!tokens) {
        printf("Lexing failed\n");
        return;
    }
    
    // Display tokens
    printf("\nTOKENS:\n");
    printf("-------\n");
    t_lex_list *token_ptr = tokens;
    int token_count = 0;
	set_the_arg_type(tokens);
	remove_quotes(tokens);
    while (token_ptr) {
        printf("Token %d: \"%s\", Type: %d, Quote: %d, Space: %d\n", 
               ++token_count, token_ptr->s, token_ptr->a_type, token_ptr->q_type, token_ptr->is_space);
        token_ptr = token_ptr->next;
    }
    
    // Parse and create AST
    t_ast_tree *ast = create_ast_tree(tokens);
    if (!ast) {
        printf("Parsing failed - returned NULL tree\n");
    } else {
        printf("\nAST Tree:\n");
        printf("---------\n");
        print_tree_detailed(ast, 0);
    }
    
    print_separator();
}

// Function to run the test
// int main(void)
// {
//     // One massive test case combining all features
//     char *mega_test = "('echo' \"complex test\" > file1.txt || (grep 'pattern' < in.txt)) && "
//                  "(cat file2.txt | sort -r | head -n 5 > out.txt) || "
//                  "((ls -la >> list.txt) && (echo \"nested quotes 'test'\" | grep test)) | "
//                  "wc -l << EOF";
    
//     test_command(mega_test);
    
//     return 0;
// }
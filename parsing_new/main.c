#include "../includes/minishell.h"

// madnsachi FREE f CASE DYALL ERRORS W CTRL D KBEL MADOZ N EXEC
// status f exit dyal syntax err khass men baed tfixiha
void	free_lex_list(t_lex_list *token)
{
	t_lex_list	*tmp;

	while (token)
	{
		tmp = token;
		free(token->s);
		token = token->next;
		free(tmp);
	}
}

void	print_lex(t_lex_list *temp)
{
	while (temp)
	{
		printf("Token: '%s', Type: %d\n", temp->s, temp->a_type);
		temp = temp->next;
	}
}

void	lopo(void)
{
	system("leaks minishell");
}

int	main(void)
{
	char *input;
	t_lex_list *tokens;
	int status = 0;
	t_ast_tree *exec_tree = NULL;
	atexit(lopo);
	while (1)
	{
		status = 0;
		input = readline("\033[32mminishell$ \033[0m");
		if (!input)
			break ;
		if (input[0])
			add_history(input);

		tokens = lexing_the_thing(input, &status);
		if (status != 0)
		{
			free(input);
			free_lex_list(tokens);
			continue ;
		}
		set_the_arg_type(tokens);
		handle_syntax_errors(tokens, &status);
		if (status != 0)
		{
			free(input);
			free_lex_list(tokens);
			continue ;
		}
		exec_tree = create_ast_tree(tokens);

		print_lex(tokens);
		free_lex_list(tokens);
		free(input);
	}

	return (0);
}
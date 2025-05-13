#include "includes/minishell.h"

/*
> bash: unexpected EOF while looking for matching `"'
bash: syntax error: unexpected end of file
*/

void	print_lex(t_lex_list *temp)
{
	while (temp)
	{
		printf("Token: |%s|, Type: %d\n", temp->s, temp->a_type);
		temp = temp->next;
	}
}

void	print_tree(t_ast_tree *tree, int deep)
{
	int			i;
	t_redirect	*t;

	if (!tree)
		return ;
	for (int i = 0; i < deep; i++)
		printf(" ");
	if (tree->args)
	{
		i = 0;
		while (tree->args[i])
		{
			printf(" |%s quote = %d is_space = %d|  ", tree->args[i],
				tree->q_type[i], tree->is_space[i]);
			i++;
		}
		printf("\n");
	}
	else
		printf("%d\n", tree->type);
	t = tree->redirect;
	// while (t)
	// {
	// 	printf("|%s| |%d| ", t->file_name, t->type);
	// 	t = t->next;
	// }
	print_tree(tree->left, deep + 1);
	print_tree(tree->right, deep + 1);
}

void	free_read_line(void)
{
	clear_history();
	// rl_clear_history();
}

int	main(int ac, char **av, char **env)
{
	char *input;
	t_lex_list *tokens;
	int status;
	t_ast_tree *astree;
	t_lex_list *lopo;
	struct sigaction sa;

	t_env_list *envv = NULL;
	set_up_env(env, &envv);

	ac = 0;
	av = NULL;
	int i = 0;
	astree = NULL;

	while (1)
	{
		input = readline("minishell$ ");

		i++;
		if (!input)
		{
			free_read_line();
			break ;
		}
		if (input[0])
			add_history(input);

		tokens = lexing_the_thing(input, &status);
		if (!tokens)
		{
			free(input);
			continue ;
		}

		set_the_arg_type(tokens);
		lopo = tokens;
		if (!handle_syntax_errors(tokens, &status))
		{
			free(input);
			free_lex_list(tokens);
			continue ;
		}
		remove_quotes(tokens);
		astree = create_ast_tree(tokens);
		free_lex_list(tokens);

		handle_heredoc(astree);
		excute_the_damn_tree(astree, &status, envv);
		free_tree(astree);

		free(input);
		if (!isatty(STDIN_FILENO))
		{
			free_read_line();
			free_env_list(envv);
			return (status);
		}
	}
	free_read_line();
	free_env_list(envv);
	return (0);
}
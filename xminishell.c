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

// Add this function to properly cleanup readline history
void cleanup_readline(void)
{
    // Clear history
    clear_history();
    // Tell readline to free its internal buffers
    rl_clear_history();
}

int	main(int ac, char **av, char **env)
{
	char		*input;
	t_lex_list	*tokens;
	int			status;
	t_ast_tree	*astree;
	t_lex_list	*lopo;
	struct sigaction sa;

	t_env_list *envv = NULL; 
	set_up_env(env, &envv);

	
	// enable_raw_mode();
	
	ac = 0;
	av = NULL;
	int i = 0;
	status = 0;
	astree = NULL;
	

	while (1)
	{
		// disable_raw_mode();
		
		input = readline("minishell$ ");
		
		// enable_raw_mode();
		
		i++;
		if (!input)
		{
			cleanup_readline();
			break ;
		}
		if (input[0])
			add_history(input);
			
		status = 0;
		
		tokens = lexing_the_thing(input, &status);
		if (status == 2)
		{
			free(input);
			free_lex_list(tokens);
			continue ;
			}
		
		set_the_arg_type(tokens);
		lopo = tokens;
		handle_syntax_errors(tokens, &status);
		if (status == 2)
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
		if (!isatty(STDIN_FILENO)) {
			cleanup_readline();
			free_env_list(envv);
			return status;
		}
	}
	cleanup_readline();
	free_env_list(envv);	
	return (0);
}
#include "includes/minishell.h"

int g_sigarette = 0;

static void clean_all_herdocs(t_ast_tree *astree)
{
	if (astree == NULL)
		return;
	t_redirect *redir = astree->redirect;
	while (redir)
	{
		close(redir->heredoc);
		redir = redir->next;
	}
	clean_all_herdocs(astree->left);
	clean_all_herdocs(astree->right);

}
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
	print_tree(tree->left, deep + 1);
	print_tree(tree->right, deep + 1);
}

void	handlectrlc(int n)
{
	g_sigarette = 130;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int	main(int ac, char **av, char **env)
{
	char *input;
	t_lex_list *tokens;
	t_ast_tree *astree;
	t_lex_list *lopo;
	t_env_list *envv = NULL;
	t_norm_m mainn;
	
	mainn.here_doc = 0;
	mainn.status = 0;
	mainn.i = 0;
	
	set_up_env(env, &envv);

	ac = 0;
	av = NULL;
	astree = NULL;

	handle_main_sigs();

	while (1)
	{	
		if (!isatty(STDIN_FILENO))
		{
			char *line = NULL;
			size_t len = 0;
			ssize_t read_len = getline(&line, &len, stdin);
			if (read_len == -1)
			{
				free(line);
				input = NULL;
			}
			else
			{
				if (line[read_len - 1] == '\n')
					line[read_len - 1] = '\0';
				input = ft_strdup(line);
				free(line);
			}
		}
		else
			input = readline("minishell$ ");
		if (g_sigarette != 0)
		{
			mainn.status = g_sigarette;
			g_sigarette = 0;
		}
		mainn.i++;
		if (!input)
		{
			free(input);
			break ;
		}
		if (input[0])
			add_history(input);

		tokens = lexing_the_thing(input, &mainn.status);
		if (!tokens)
		{
			free(input);
			continue ;
		}

		set_the_arg_type(tokens);
		lopo = tokens;
		if (!handle_syntax_errors(tokens, &mainn.status))
		{
			free(input);
			free_lex_list(tokens);
			continue ;
		}
		remove_quotes(tokens);
		astree = create_ast_tree(tokens);
		free_lex_list(tokens);

		if (handle_heredoc(astree, 0, envv) == -1)
		{
			if (g_sigarette == 130)
			{
				mainn.status = 1;
				g_sigarette = 0;
			}
			free(input);
			free_tree(astree);
			continue;
		}
		excute_the_damn_tree(astree, &mainn.status, &envv,0);
		clean_all_herdocs(astree);
		free_tree(astree);
		free(input);
		if (!isatty(STDIN_FILENO))
		{
			rl_clear_history();
			free_env_list(envv);
			return (mainn.status);
		}
	}
	rl_clear_history();
	free_env_list(envv);
	return (mainn.status);
}
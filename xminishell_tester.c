#include "includes/minishell.h"

int sigarette = 0;
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
/*
> bash: unexpected EOF while looking for matching `"'
bash: syntax error: unexpected end of file
*/
	// }
	print_tree(tree->left, deep + 1);
	print_tree(tree->right, deep + 1);
}

void	handlectrlc(int n)
{
	sigarette = 130;
	printf("\n");
	rl_on_new_line();
	// rl_replace_line("", 0);
	rl_redisplay();
}

int	main(int ac, char **av, char **env)
{
	char *input;
	int here_doc = 0;
	t_lex_list *tokens;
	int status = 0;
	t_ast_tree *astree;
	t_lex_list *lopo;
	
	t_env_list *envv = NULL;
	set_up_env(env, &envv);

	ac = 0;
	av = NULL;
	int i = 0;
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
		if (sigarette != 0)
		{
			status = sigarette;
			sigarette = 0;
		}
		i++;
		if (!input)
		{
			free(input);
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

		if (handle_heredoc(astree, 0, envv) == -1)
		{
			if (sigarette == 130)
			{
				status = 1;
				sigarette = 0;
			}
			free(input);
			free_tree(astree);
			continue;
		}
		excute_the_damn_tree(astree, &status, &envv);
		clean_all_herdocs(astree);
		free_tree(astree);
		free(input);
		//is this a must ? check before pushing
		if (!isatty(STDIN_FILENO))
		{
			// rl_clear_history();
			free_env_list(envv);
			return (status);
		}
	}
	// rl_clear_history();
	free_env_list(envv);
	return (status);
}
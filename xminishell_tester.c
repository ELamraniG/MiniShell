#include "includes/minishell.h"

int			g_sigarette = 0;

static void	clean_all_herdocs(t_ast_tree *astree)
{
	t_redirect	*redir;

	if (astree == NULL)
		return ;
	redir = astree->redirect;
	while (redir)
	{
		if (redir->heredoc != -1)
			close(redir->heredoc);
		redir = redir->next;
	}
	clean_all_herdocs(astree->left);
	clean_all_herdocs(astree->right);
}

void	init_norm(t_norm_m *mainn, int ac, char **av, t_ast_tree **astree)
{
	(void)ac;
	(void)av;
	mainn->here_doc = 0;
	mainn->status = 0;
	*astree = NULL;
	handle_main_sigs();
}

void	handle_input(char **input)
{
	char	*line;
	size_t	len;
	ssize_t	read_len;

	if (!isatty(STDIN_FILENO))
	{
		line = NULL;
		len = 0;
		read_len = getline(&line, &len, stdin);
		if (read_len == -1)
		{
			free(line);
			*input = NULL;
		}
		else
		{
			if (line[read_len - 1] == '\n')
				line[read_len - 1] = '\0';
			*input = ft_strdup(line);
			free(line);
		}
	}
	else
		*input = readline("minishell$ ");
}

void	cleanup_and_exit(t_env_list **envv)
{
	rl_clear_history();
	free_env_list(*envv);
}

void	execute_and_cleanup(t_ast_tree **astree, int *status,
		t_env_list **envv, char **input)
{
	excute_the_damn_tree(*astree, status, envv, 0);
	clean_all_herdocs(*astree);
	free_tree(*astree);
	free(*input);
}

void	process_tokens(t_lex_list **tokens, t_ast_tree **astree)
{
	remove_quotes(*tokens);
	*astree = create_ast_tree(*tokens);
	free_lex_list(*tokens);
}

void	handle_signal_status(int *status)
{
	*status = g_sigarette;
	g_sigarette = 0;
}

void	handle_heredoc_signal(int *status)
{
	*status = 1;
	g_sigarette = 0;
}

int	handle_lexing(char **input, t_lex_list **tokens, int *status)
{
	*tokens = lexing_the_thing(*input, status);
	if (!*tokens)
	{
		free(*input);
		return (-1);
	}
	set_the_arg_type(*tokens);
	return (0);
}

int	handle_syntax_check(t_lex_list **tokens, int *status, char **input)
{
	if (!handle_syntax_errors(*tokens, status))
	{
		free(*input);
		free_lex_list(*tokens);
		return (0);
	}
	return (1);
}

int	handle_heredoc_processing(t_ast_tree *astree, t_env_list *envv,
		char **input, int *status)
{
	if (handle_heredoc(astree, 0, envv) == -1)
	{
		if (g_sigarette == 130)
			handle_heredoc_signal(status);
		free(*input);
		free_tree(astree);
		return (-1);
	}
	return (0);
}

int	main_loop_iteration(t_env_list **envv, t_norm_m *mainn)
{
	char		*input;
	t_lex_list	*tokens;
	t_ast_tree	*astree;

	handle_input(&input);
	if (g_sigarette != 0)
		handle_signal_status(&mainn->status);
	if (!input)
	{
		free(input);
		return (1);
	}
	if (input[0])
		add_history(input);
	if (handle_lexing(&input, &tokens, &mainn->status) == -1)
		return (0);
	if (!handle_syntax_check(&tokens, &mainn->status, &input))
		return (0);
	process_tokens(&tokens, &astree);
	if (handle_heredoc_processing(astree, *envv, &input, &mainn->status) == -1)
		return (0);
	execute_and_cleanup(&astree, &mainn->status, envv, &input);
	if (!isatty(STDIN_FILENO))
	{
		cleanup_and_exit(envv);
		exit(mainn->status);
	}
	return (0);
}

int	main(int ac, char **av, char **env)
{
	t_env_list	*envv;
	t_norm_m	mainn;
	t_ast_tree	*astree;

	envv = NULL;
	init_norm(&mainn, ac, av, &astree);
	set_up_env(env, &envv);
	while (1)
	{
		if (main_loop_iteration(&envv, &mainn))
			break ;
	}
	cleanup_and_exit(&envv);
	return (mainn.status);
}
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
		close(redir->heredoc);
		redir = redir->next;
	}
	clean_all_herdocs(astree->left);
	clean_all_herdocs(astree->right);
}

void	init_norm(t_norm_m *mainn, int ac, char **av, t_ast_tree **astree)
{
	ac = 0;
	av = NULL;
	mainn->here_doc = 0;
	mainn->status = 0;
	*astree = NULL;
	handle_main_sigs();
}

void	is_tty(char **input)
{
	if (!isatty(STDIN_FILENO))
		*input = readline("");
	else
		*input = readline("minishell$ ");
}

void	is_tt2(t_env_list **env)
{
	rl_clear_history();
	free_env_list(*env);
}

void	exec_tree_cleandoc_free_tree_frinput(t_ast_tree **astree, int *status,
		t_env_list **envv, char **input)
{
	excute_the_damn_tree(*astree, status, envv, 0);
	clean_all_herdocs(*astree);
	free_tree(*astree);
	free(*input);
}

void	norm_tre_tok(t_lex_list **tokens, t_ast_tree **astree)
{
	remove_quotes(*tokens);
	*astree = create_ast_tree(*tokens);
	free_lex_list(*tokens);
}

void	norm_sig(int *status)
{
	*status = g_sigarette;
	g_sigarette = 0;
}

void	norm_sig12(int *status)
{
	*status = 1;
	g_sigarette = 0;
}
int	handle_token(t_lex_list **tokens, char **input, int *status)
{
	*tokens = lexing_the_thing(*input, status);
	if (!tokens)
	{
		free(*input);
		return (-1);
	}
	set_the_arg_type(*tokens);
	return (0);
}

		syntax_error_norminated(t_lex_list **tokens,int *status,char **input)
		{
			if (!handle_syntax_errors(*tokens, status))
		{
			free(*input);
			free_lex_list(*tokens);
			return 0;
		}
		return 1;
		}

int	main(int ac, char **av, char **env)
{
	char		*input;
	t_lex_list	*tokens;
	t_ast_tree	*astree;
	t_env_list	*envv;
	t_norm_m	mainn;

	init_norm(&mainn, ac, av, &astree);
	set_up_env(env, &envv);
	while (1337)
	{
		is_tty(&input);
		if (g_sigarette != 0)
			norm_sig(&mainn.status);
		if (!input)
			break ;
		if (input[0])
			add_history(input);
		if (handle_token(&tokens, &input, &mainn.status) == -1)
			continue ;
		if (!syntax_error_norminated(&tokens,&mainn.status,&input))
			continue;
		// if (!handle_syntax_errors(tokens, &mainn.status))
		// {
		// 	free(input);
		// 	free_lex_list(tokens);
		// 	continue ;
		// }
		norm_tre_tok(&tokens, &astree);
		if (handle_heredoc(astree, 0, envv) == -1)
		{
			if (g_sigarette == 130)
				norm_sig12(&mainn.status);
			free(input);
			free_tree(astree);
			continue ;
		}
		exec_tree_cleandoc_free_tree_frinput(&astree, &mainn.status, &envv,
			&input);
		if (!isatty(STDIN_FILENO))
			return (is_tt2(&envv), mainn.status);
	}
	return (is_tt2(&envv), mainn.status);
}

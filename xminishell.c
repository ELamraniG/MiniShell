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

void	exec_tree_cleandoc_free_tree_frinput(t_ast_tree **astree, int *status,
		t_env_list **envv,char **input)
{
	excute_the_damn_tree(*astree, status, envv, 0);
	clean_all_herdocs(*astree);
	free_tree(*astree);
	free(*input);
}

int	main(int ac, char **av, char **env)
{
	char		*input;
	t_lex_list	*tokens;
	t_ast_tree	*astree;
	t_env_list	*envv;
	t_norm_m	mainn;

	envv = NULL;
	init_norm(&mainn, ac, av, &astree);
	set_up_env(env, &envv);
	while (1337)
	{
		is_tty(&input);
		if (g_sigarette != 0)
		{
			mainn.status = g_sigarette;
			g_sigarette = 0;
		}
		if (!input)
			break ;
		if (input[0])
			add_history(input);
		tokens = lexing_the_thing(input, &mainn.status);
		if (!tokens)
		{
			free(input);
			continue ;
		}
		set_the_arg_type(tokens);
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
			continue ;
		}
		exec_tree_cleandoc_free_tree_frinput(&astree, &mainn.status,&envv,&input);
		// excute_the_damn_tree(astree, &mainn.status, &envv, 0);
		// clean_all_herdocs(astree);
		// free_tree(astree);
		// free(input);
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

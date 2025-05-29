/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xminishell_continue_1_bonus.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 22:42:16 by jhamdaou          #+#    #+#             */
/*   Updated: 2025/05/29 23:11:26 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minishell_bonus.h"

void	clean_all_herdocs(t_ast_tree *astree)
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

void	exec_tree_ndrest(t_ast_tree **astree, int *status, t_env_list **envv,
		char **input)
{
	excute_the_damn_tree(*astree, status, envv, 0);
	clean_all_herdocs(*astree);
	free_tree(*astree);
	free(*input);
}

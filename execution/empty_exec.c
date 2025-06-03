/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   norminette_exec_tree.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-amr <moel-amr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 22:51:09 by jhamdaou          #+#    #+#             */
/*   Updated: 2025/05/29 22:37:33 by moel-amr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"



static void	handle_andd(t_ast_tree *astree, int *status, t_env_list **env,
		int in_pipe)
{
	excute_the_damn_tree(astree->left, status, env, in_pipe);
	if (*status == 0)
		excute_the_damn_tree(astree->right, status, env, in_pipe);
}

static void	handle_orr(t_ast_tree *astree, int *status, t_env_list **env,
		int in_pipe)
{
	excute_the_damn_tree(astree->left, status, env, in_pipe);
	if (*status != 0)
		excute_the_damn_tree(astree->right, status, env, in_pipe);
}

void	empty_tree(t_ast_tree *astree, int *status, t_env_list **env,
		int in_pipe)
{
	int		pipes[2];

	if (!astree)
		return ;
	if (astree->type == PIPE)
		empty_pipe(astree, pipes, status, env);
	else if (astree->type == AND)
		handle_andd(astree, status, env, in_pipe);
	else if (astree->type == OR)
		handle_orr(astree, status, env, in_pipe);
}

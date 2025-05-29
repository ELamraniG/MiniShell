/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   norminette_word1_continue_bonus.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:26:58 by jhamdaou          #+#    #+#             */
/*   Updated: 2025/05/29 23:10:57 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minishell_bonus.h"

void	norm_word_166(t_ast_tree *astree, t_env_list **env,
							char ***env_char)
{
	struct stat	l;
	int			p;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	p = handle_path(astree->args, *env);
	if (p == -1)
		print_msg1(astree->args[0]);
	else if (p == -2)
		print_msg2(astree->args[0]);
	else if (p == -3)
		print_msg3(astree->args[0]);
	if (stat(astree->args[0], &l) == 0 && S_ISDIR(l.st_mode))
		print_msg4(astree->args[0]);
	*env_char = turn_env_to_chars(*env);
}

void	failed_exec_v(t_ast_tree *astree, char ***env_char)
{
	int	i;

	i = 0;
	perror(astree->args[0]);
	if (*env_char)
	{
		while (env_char[0][i])
			free(env_char[0][i++]);
		free(*env_char);
	}
	exit(127);
}

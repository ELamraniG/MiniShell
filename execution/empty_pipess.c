/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-amr <moel-amr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:48:01 by moel-amr          #+#    #+#             */
/*   Updated: 2025/05/29 22:37:16 by moel-amr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	empty_pipe(t_ast_tree *astree, int *pipes, int *status,
		t_env_list **env)
{
	int	pid1;
	int	pid2;
	int exit_code;
	if (pipe(pipes) == -1)
	{
		*status = 1;
		perror(NULL);
		return ;
	}
	close(pipes[0]);
	close(pipes[1]);
	pid1 = fork();
	if (pid1 == -1)
	{
		ft_putstr_fd(2,"here01\n\n");
		exit(1);
	}
	if (pid1 == 0)
	{
		if (astree->left && astree->left->type == PIPE)
			empty_tree(astree->left,status,env,0);
		exit(*status);
	}
	waitpid(pid1,&exit_code,0);
	if (WIFEXITED(exit_code))
		*status = WEXITSTATUS(exit_code);
	else if (WIFSIGNALED(exit_code))
		*status = 128 + WTERMSIG(exit_code);
	if (*status == 1)
		return ;
	pid2 = fork();
	if (pid2 == -1)
	{
		ft_putstr_fd(2,"here\n\n");
		exit(1);
	}
	if (pid2 == 0)
	{
		if (astree->right && astree->left->type == PIPE)
			empty_tree(astree->right,status,env,0);
		exit(*status);
	}
	waitpid(pid2,&exit_code,0);
	if (WIFEXITED(exit_code))
		*status = WEXITSTATUS(exit_code);
	else if (WIFSIGNALED(exit_code))
		*status = 128 + WTERMSIG(exit_code);
	ft_putstr_fd(2,ft_itoa(*status));
	ft_putstr_fd(2,"\n");
	if (*status == 1)
		return ;
}

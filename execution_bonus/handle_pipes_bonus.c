/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipes_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:48:01 by moel-amr          #+#    #+#             */
/*   Updated: 2025/05/29 23:10:01 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minishell_bonus.h"

static void	norminette_handlepipe1(int *pipes, int *status)
{
	close(pipes[0]);
	close(pipes[1]);
	*status = 1;
	perror(NULL);
}

static void	norminette_handlepipe2(int *pipes, int *exit_code, int pid1,
		int pid2)
{
	close(pipes[0]);
	close(pipes[1]);
	ignore_signals();
	waitpid(pid1, NULL, 0);
	waitpid(pid2, exit_code, 0);
	handle_main_sigs();
}

static void	norminette_handlepipe3(int exit_code, int *status)
{
	if (WIFEXITED(exit_code))
		*status = WEXITSTATUS(exit_code);
	else if (WIFSIGNALED(exit_code))
		*status = 128 + WTERMSIG(exit_code);
}

static int	norm_handlepipe4(t_ast_tree *astree, int *pipes, int *status,
		t_env_list **env)
{
	int	pid2;

	pid2 = fork();
	if (pid2 == -1)
	{
		norminette_handlepipe1(pipes, status);
		return (-1);
	}
	if (pid2 == 0)
	{
		dup3(pipes[0], STDIN_FILENO);
		close(pipes[1]);
		excute_the_damn_tree(astree->right, status, env, 1);
		exit(*status);
	}
	return (pid2);
}

void	handle_pipe(t_ast_tree *astree, int *pipes, int *status,
		t_env_list **env)
{
	int	exit_code;
	int	pid1;
	int	pid2;

	if (pipe(pipes) == -1)
		return (*status = 1, perror(NULL));
	pid1 = fork();
	if (pid1 == -1)
	{
		norminette_handlepipe1(pipes, status);
		return ;
	}
	if (pid1 == 0)
	{
		dup3(pipes[1], STDOUT_FILENO);
		close(pipes[0]);
		excute_the_damn_tree(astree->left, status, env, 1);
		exit(*status);
	}
	pid2 = norm_handlepipe4(astree, pipes, status, env);
	if (pid2 == -1)
		return ;
	norminette_handlepipe2(pipes, &exit_code, pid1, pid2);
	norminette_handlepipe3(exit_code, status);
}

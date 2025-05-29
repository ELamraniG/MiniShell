/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   norminette_word1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:59:48 by moel-amr          #+#    #+#             */
/*   Updated: 2025/05/29 16:26:45 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	word_norminette(t_ast_tree *astree, t_env_list **env, int *status,
		t_stdindo stddd)
{
	I_HATE_EXPANDING(astree, *env, *status);
	if (expand_file_name(astree, *env, *status) == -1)
	{
		*status = 1;
		dup3(stddd.stdinn, STDIN_FILENO);
		dup3(stddd.stdoutt, STDOUT_FILENO);
		return (-1);
	}
	handle_wild_card(astree);
	if (handle_file_wildcard(astree) == -1)
	{
		*status = 1;
		dup3(stddd.stdinn, STDIN_FILENO);
		dup3(stddd.stdoutt, STDOUT_FILENO);
		return (-1);
	}
	astree->args = join_args_without_spaces(astree);
	join_all_redir_files_without_spaces(astree);
	return (0);
}

static int	word_norminette2(t_ast_tree *astree, t_stdindo stddd, int *status,
		t_env_list **env)
{
	if (excute_redirs(astree) == -1)
	{
		dup3(stddd.stdinn, STDIN_FILENO);
		dup3(stddd.stdoutt, STDOUT_FILENO);
		*status = 1;
		return (-1);
	}
	if (is_built_in(astree->args[0]))
	{
		*status = execute_built_in(astree->args, env, status);
		dup3(stddd.stdinn, STDIN_FILENO);
		dup3(stddd.stdoutt, STDOUT_FILENO);
		return (-1);
	}
	return (0);
}

static void	norm_word4(t_stdindo stddd, int *status, int exit_code)
{
	handle_main_sigs();
	if (WIFEXITED(exit_code))
		*status = WEXITSTATUS(exit_code);
	else if (WIFSIGNALED(exit_code))
	{
		*status = 128 + WTERMSIG(exit_code);
		if (WTERMSIG(exit_code) == SIGQUIT)
			ft_putstr_fd(2, "Quit: 3\n");
		if (WTERMSIG(exit_code) == SIGINT)
			ft_putstr_fd(2, "\n");
	}
	dup3(stddd.stdinn, STDIN_FILENO);
	dup3(stddd.stdoutt, STDOUT_FILENO);
}

static void	norm_word3(t_stdindo stddd, int *status)
{
	close(stddd.stdinn);
	close(stddd.stdoutt);
	*status = 1;
	perror(NULL);
}

void	norminette_handle_word1(t_ast_tree *astree, t_env_list **env,
		int *status)
{
	t_stdindo	stddd;
	int			pid1;
	int			exit_code;
	char		**env_char;

	stddd.stdinn = dup(STDIN_FILENO);
	stddd.stdoutt = dup(STDOUT_FILENO);
	if (word_norminette(astree, env, status, stddd) == -1
		|| word_norminette2(astree, stddd, status, env) == -1)
		return ;
	pid1 = fork();
	if (pid1 == -1)
		return (norm_word3(stddd, status));
	if (pid1 == 0)
	{
		norm_word_166(astree, env, &env_char);
		execve(astree->args[0], astree->args, env_char);
		failed_exec_v(astree, &env_char);
	}
	ignore_signals();
	waitpid(pid1, &exit_code, 0);
	norm_word4(stddd, status, exit_code);
}

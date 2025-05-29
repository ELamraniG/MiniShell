/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   norminette_word2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:33:04 by jhamdaou          #+#    #+#             */
/*   Updated: 2025/05/29 16:42:12 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	word_norm(t_ast_tree *astree, t_env_list **env, int *status,
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

static int	word_norm2(t_ast_tree *astree, t_stdindo stddd, int *status,
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

static void	norminette_word_166(t_ast_tree *astree, t_env_list **env,
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

static void	failed_execute_v(t_ast_tree *astree, char ***env_char)
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

void	norminette_handle_word2(t_ast_tree *astree, t_env_list **env,
		int *status)
{
	t_stdindo	stddd;
	char		**env_char;

	stddd.stdinn = dup(STDIN_FILENO);
	stddd.stdoutt = dup(STDOUT_FILENO);
	if (word_norm(astree, env, status, stddd) == -1 || word_norm2(astree, stddd,
			status, env) == -1)
		return ;
	norminette_word_166(astree, env, &env_char);
	execve(astree->args[0], astree->args, env_char);
	failed_execute_v(astree, &env_char);
}

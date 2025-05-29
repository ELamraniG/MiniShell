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

char	**turn_env_to_chars(t_env_list *env)
{
	char		**s;
	char		*tmp_free;
	t_env_list	*tmp;
	int			i;

	tmp = env;
	i = 0;
	while (env)
	{
		i++;
		env = env->next;
	}
	if (i == 0)
		return (NULL);
	s = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (tmp)
	{
		s[i] = ft_strjoin(tmp->key, "=");
		tmp_free = s[i];
		s[i] = ft_strjoin(s[i], tmp->value);
		i++;
		normeee(&tmp, tmp_free);
	}
	return (s[i] = NULL, s);
}

void	handle_andd(t_ast_tree *astree, int *status, t_env_list **env,
		int in_pipe)
{
	excute_the_damn_tree(astree->left, status, env, in_pipe);
	if (*status == 0)
		excute_the_damn_tree(astree->right, status, env, in_pipe);
}

void	handle_orr(t_ast_tree *astree, int *status, t_env_list **env,
		int in_pipe)
{
	excute_the_damn_tree(astree->left, status, env, in_pipe);
	if (*status != 0)
		excute_the_damn_tree(astree->right, status, env, in_pipe);
}

void	handle_empty_command_with_redir(t_ast_tree *astree, int *status,
		t_env_list **env, int in_pipe)
{
	int	stdinn;
	int	stdoutt;

	stdinn = dup(STDIN_FILENO);
	stdoutt = dup(STDOUT_FILENO);
	if (expand_file_name(astree, *env, *status) == -1)
	{
		*status = 1;
		return (dup3(stdoutt, STDOUT_FILENO), dup3(stdinn, STDIN_FILENO));
	}
	join_all_redir_files_without_spaces(astree);
	if (excute_redirs(astree) == -1)
	{
		dup3(stdinn, STDIN_FILENO);
		*status = 1;
		return (dup3(stdoutt, STDOUT_FILENO));
	}
	if (astree->left)
		excute_the_damn_tree(astree->left, status, env, in_pipe);
	else if (astree->right)
		excute_the_damn_tree(astree->right, status, env, in_pipe);
	else
		*status = 0;
	dup3(stdinn, STDIN_FILENO);
	dup3(stdoutt, STDOUT_FILENO);
}

void	excute_the_damn_tree(t_ast_tree *astree, int *status, t_env_list **env,
		int in_pipe)
{
	int		pipes[2];

	if (!astree)
		return ;
	if (astree->type == PIPE)
		handle_pipe(astree, pipes, status, env);
	else if (astree->type == AND)
		handle_andd(astree, status, env, in_pipe);
	else if (astree->type == OR)
		handle_orr(astree, status, env, in_pipe);
	else if (astree->type < 4)
		handle_empty_command_with_redir(astree, status, env, in_pipe);
	else if (astree->type == WORD)
	{
		if (in_pipe)
			norminette_handle_word2(astree, env, status);
		else
			norminette_handle_word1(astree, env, status);
	}
}

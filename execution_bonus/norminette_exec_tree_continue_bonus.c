/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   norminette_exec_tree_continue_bonus.c              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 22:52:14 by jhamdaou          #+#    #+#             */
/*   Updated: 2025/05/29 23:10:47 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minishell_bonus.h"

void	dup3(int new, int original)
{
	dup2(new, original);
	close(new);
}

int	path_norm1(char **args)
{
	if (!args || !args[0] || !*args[0])
		return (-1);
	if (ft_strchr(args[0], '/'))
	{
		if (!access(args[0], X_OK))
			return (1);
		if (access(args[0], F_OK) == 0)
			return (-2);
		return (-1);
	}
	return (0);
}

int	norm_path2(char **splited_path, char **args, int i)
{
	char	*tmp_free;
	char	*tmp_join;

	tmp_free = splited_path[i];
	splited_path[i] = ft_strjoin(splited_path[i], "/");
	free(tmp_free);
	tmp_join = ft_strjoin(splited_path[i], args[0]);
	if (!access(tmp_join, X_OK))
	{
		tmp_free = args[0];
		args[0] = tmp_join;
		i = 0;
		while (splited_path[i])
			free(splited_path[i++]);
		free(splited_path);
		return (1);
	}
	free(tmp_join);
	return (0);
}

int	handle_path(char **args, t_env_list *env)
{
	int		i;
	char	*path;
	char	**splited_path;
	int		kk;

	i = 0;
	kk = path_norm1(args);
	if (kk != 0)
		return (kk);
	env = get_env_value(env, "PATH");
	if (!env || !env->value || !env->value[0])
		return (-3);
	path = env->value;
	if (path == NULL || !*path)
		return (-1);
	splited_path = ft_split(path, ':');
	while (splited_path[i])
	{
		if (norm_path2(splited_path, args, i++) == 1)
			return (1);
	}
	i = 0;
	while (splited_path[i])
		free(splited_path[i++]);
	return (free(splited_path), -1);
}

void	normeee(t_env_list **t, char *tmp)
{
	free(tmp);
	*t = (*t)->next;
}

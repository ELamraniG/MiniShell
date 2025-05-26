/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:46:43 by jhamdaou          #+#    #+#             */
/*   Updated: 2025/05/26 17:54:52 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_normal(char *args, t_env_list **env, int j)
{
	char	*key;
	char	*value;

	key = ft_substr(args, 0, j);
	if (!validate_key(key))
	{
		print_msg(key);
		free (key);
		return (1);
	}
	value = ft_substr(args, j + 1, ft_strlen(args) - j);
	insert_node_last(env, key, value, 1);
	free(key);
	free(value);
	return (0);
}

int	handle_join(char *args, t_env_list **env, int j)
{
	char		*key;
	char		*value;
	char		*new_val;
	t_env_list	*exists;

	key = ft_substr(args, 0, j - 1);
	if (!validate_key(key))
	{
		print_msg(args);
		free(key);
		return (1);
	}
	value = ft_substr(args, j + 1, ft_strlen(args) - j);
	exists = get_env_value(*env, key);
	if (exists)
	{
		new_val = ft_strjoin(exists->value, value);
		free (exists->value);
		exists->value = new_val;
		exists->flag = 1;
	}
	else
		insert_node_last(env, key, value, 1);
	return (free(key), free(value), 0);
}

int	handle_key_alone(char *args, t_env_list **env)
{
	char		*key;
	t_env_list	*exists;

	key = ft_strdup(args);
	if (!validate_key(key))
	{
		print_msg(key);
		free(key);
		return (1);
	}
	exists = get_env_value(*env, key);
	if (!exists)
		insert_node_last(env, key, "", 0);
	free(key);
	return (0);
}

int	handle_export(char **args, t_env_list **env, int i, int *j)
{
	while (args[i][++(*j)])
	{
		if (args[i][*j] == '=')
		{
			if (*j == 0)
			{
				print_msg(args[i]);
				break ;
			}
			if (j > 0 && args[i][(*j) - 1] == '+')
			{
				if (handle_join(args[i], env, *j) == 1)
					break ;
			}
			else
			{
				if (handle_normal(args[i], env, *j) == 1)
					break ;
			}
			break ;
		}
	}
	return (0);
}

int	exec_export(t_env_list **env, char **args)
{
	int	i;
	int	j;

	i = 0;
	if (!args[1])
		return (print_export(*env), 0);
	while (args[++i])
	{
		j = -1;
		handle_export(args, env, i, &j);
		if (!args[i][j] && j > 0)
			handle_key_alone(args[i], env);
	}
	return (0);
}

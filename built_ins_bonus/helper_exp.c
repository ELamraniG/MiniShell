/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_exp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:46:45 by jhamdaou          #+#    #+#             */
/*   Updated: 2025/05/26 17:49:32 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_msg(char *args)
{
	ft_putstr_fd(2, "minishell: export: `");
	ft_putstr_fd(2, args);
	ft_putstr_fd(2, "': not a valid identifier\n");
}

void	print_export(t_env_list *env_list)
{
	while (env_list)
	{
		if (env_list->key)
		{
			printf("declare -x %s", env_list->key);
			if (env_list->flag)
				printf("=\"%s\"\n", env_list->value);
			else
				printf("\n");
		}
		env_list = env_list->next;
	}
}

int	validate_key(char *s)
{
	int	i;

	i = 0;
	if (!ft_isalpha(s[i]) && s[i] != '_')
		return (0);
	i++;
	while (s[i])
	{
		if (s[i] != '_' && !ft_isalnum(s[i]))
			return (0);
		i++;
	}
	return (1);
}

char	*get_value_env(t_env_list**d, char *key)
{
	t_env_list	*env;

	if (!key)
		return (NULL);
	env = *d;
	while (env)
	{
		if (!ft_strcmp(env->key, key))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

int	find_key(t_env_list *d, char *key)
{
	while (d)
	{
		if (!ft_strcmp(d->key, key))
			return (1);
		d = d->next;
	}
	return (0);
}

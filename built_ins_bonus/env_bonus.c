/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:46:29 by jhamdaou          #+#    #+#             */
/*   Updated: 2025/05/29 23:07:55 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minishell_bonus.h"

t_env_list	*new_env_node(void)
{
	t_env_list	*env_node;

	env_node = malloc(sizeof(t_env_list));
	if (!env_node)
		return (0);
	env_node->key = NULL;
	env_node->value = NULL;
	env_node->next = NULL;
	env_node->flag = 0;
	return (env_node);
}

t_env_list	*create_env(char *ev, char *pos)
{
	t_env_list	*new;

	new = new_env_node();
	if (!new)
		return (NULL);
	new->key = ft_substr(ev, 0, pos - ev);
	new->value = ft_substr(ev, (pos - ev) + 1, ft_strlen(ev) - (pos - ev) - 1);
	if (!new->key || !new->value)
	{
		free(new->key);
		free(new->value);
		free(new);
		return (NULL);
	}
	new->flag = 1;
	new->next = NULL;
	return (new);
}

void	process_env_loop(char **env, t_env_list **env_list)
{
	char		*eq_p;
	int			i;
	t_env_list	*new;
	t_env_list	*last;

	i = -1;
	last = NULL;
	while (env[++i])
	{
		eq_p = ft_strchr(env[i], '=');
		if (eq_p)
		{
			new = create_env(env[i], eq_p);
			if (new)
			{
				if (!(*env_list))
					*env_list = new;
				else
					last->next = new;
				last = new;
			}
		}
	}
}

void	set_up_env(char **env, t_env_list **env_list)
{
	char	*cwd;

	if (!env_list)
		return ;
	*env_list = NULL;
	if (!env || !env[0])
	{
		cwd = getcwd(NULL, 0);
		insert_node(env_list, ft_strdup("PWD"), cwd, 1);
		insert_node(env_list, ft_strdup("SHLVL"), ft_strdup("1"), 1);
		insert_node(env_list, ft_strdup("_"), ft_strdup("/usr/bin/env"), 1);
		insert_node(env_list, ft_strdup("PATH"),
			ft_strdup("/usr/local/bin:/usr/bin:/bin"), 1);
	}
	else
		process_env_loop(env, env_list);
}

int	print_env(t_env_list *env_list)
{
	if (!env_list)
		return (1);
	while (env_list)
	{
		if (env_list->flag)
			printf("%s=%s\n", env_list->key, env_list->value);
		env_list = env_list->next;
	}
	return (0);
}

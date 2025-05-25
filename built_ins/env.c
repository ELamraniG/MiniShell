#include "../includes/minishell.h"

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

void	set_up_env(char **env, t_env_list **env_list)
{
	int			i;
	char		*eq_p;
	t_env_list	*new;
	t_env_list	*last;

	i = 0;
	last = NULL;
	if (!env || !*env || !env_list)
		return ;
	*env_list = NULL;
	while (env[i])
	{
		eq_p = ft_strchr(env[i], '=');
		if (eq_p)
		{
			new = new_env_node();
			if (!new)
				return ;
			new->key = ft_substr(env[i], 0, eq_p - env[i]);
			new->value = ft_substr(env[i], (eq_p - env[i]) + 1, ft_strlen(env[i]) - (eq_p - env[i]) - 1);
			if (!new->key || !new->value)
			{
				free(new->key);
				free(new->value);
				free(new);
				return ;
			}
			new->flag = 1;
			if (!(*env_list))
				*env_list = new;
			else
				last->next = new;
			last = new;
		}
		i++;
	}
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

void	free_env_list(t_env_list *env_list)
{
	t_env_list	*tmp;

	while (env_list)
	{
		tmp = env_list;
		env_list = env_list->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}

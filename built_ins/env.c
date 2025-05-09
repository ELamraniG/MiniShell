#include "../includes/minishell.h"

t_env_list	*new_env_node()
{
	t_env_list	*env_node;

	env_node = malloc(sizeof(t_env_list));
	if (!env_node)
		return (0);
	env_node->key = NULL;
	env_node->value = NULL;
	env_node->next = NULL;
	env_node->flag = 0;
	return(env_node);
}

void	set_up_env(char **env, t_env_list **env_list)
{
	int	i;
	t_env_list	*new_node;
	t_env_list	*last;
	char *equals_pos;

	i = 0;
	last = NULL;
	if (!env || !*env || !env_list)
		return ;
	*env_list = NULL; // Initialize the list pointer
	while (env[i])
	{
		equals_pos = ft_strchr(env[i], '=');
		if (equals_pos)
		{
			new_node = new_env_node();
			if (!new_node)
				return ;
			new_node->key = ft_substr(env[i], 0, equals_pos - env[i]);
			new_node->value = ft_substr(env[i], (equals_pos - env[i]) + 1, ft_strlen(env[i]) - (equals_pos - env[i]) - 1);
			if (!new_node->key || !new_node->value)
			{
				free(new_node->key);
				free(new_node->value);
				free(new_node);
				return ;
			}
			new_node->flag = 1;
			if (!(*env_list))
				*env_list = new_node;
			else
				last->next = new_node;
			last = new_node;
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
void free_env_list(t_env_list *env_list)
{
	t_env_list *tmp;

	while (env_list)
	{
		tmp = env_list;
		env_list = env_list->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}
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
	return(env_node);
}

void	set_up_env(char **env, t_env_list **env_list)
{
	int	i = 0;
	int	j = 0;
	t_env_list	*new_node;
	t_env_list	*last = NULL;
	while (env[i])
	{
		j = 0;
		while (env[i][j])
		{
			if (env[i][j] == '=')
			{
				new_node = new_env_node();
				new_node->key = ft_substr(env[i], 0, j + 1);
				new_node->value = ft_substr(env[i], j + 1, ft_strlen(env[i]) - j);
				if (!(*env_list))
					*env_list = new_node;
				else
					last->next = new_node;
				last = new_node;
			}
			j++;
		}
		i++;
	}
}


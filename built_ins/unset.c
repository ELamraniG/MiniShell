#include "../includes/minishell.h"

int	exec_unset(t_env_list **env, char **args)
{
	int	i;
	int	flag;

	flag = 0;
	if (!args[1])
		return (0); 
	i = 1;
	while (args[i])
	{
		if (!validate_key(args[i]))
		{
			printf("minishell: unset: `%s': not a valid identifier\n", args[i]);
			flag = 1;
		}
		else
			delete_node(env, args[i]);
		i++;
	}
	if (flag)
		return (1);
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
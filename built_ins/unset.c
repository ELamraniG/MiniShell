#include "../includes/minishell.h"

int	exec_unset(t_env_list **env, char **args)
{
	int	i;

	if (!args[1])
		return (0); // Nothing to unset, success
	for (i = 1; args[i]; i++)
	{
		delete_node(env, args[i]);
	}
	return (0); // Success
}

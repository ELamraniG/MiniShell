#include "../includes/minishell.h"

static int	validate_key(char *s)
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
		delete_node(env, args[i]);
		i++;
	}
	if (flag)
		return (1);
	return (0); 
}

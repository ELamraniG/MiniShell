#include "../includes/minishell.h"

int	change_dir(char **args, t_env_list *env)
{
	if (!args ||  !args[0] || ft_strcmp(args[0], "cd") != 0)
		return 1;
	chdir(args[1]);
	return 0;
}

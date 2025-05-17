#include "../includes/minishell.h"

int	change_dir(char **args, t_env_list *env)
{
	int			val;
	t_env_list *home;

	home = get_env_value(env, "HOME");
	if (!args ||  !args[0] || ft_strcmp(args[0], "cd") != 0)
		return 1;
	val = chdir(args[1]);
	if (!args[1] && get_env_value(env, "HOME") == NULL) // for if we unset HOME
	{
		ft_putstr_fd(2, "minishell: cd: HOME not set\n");
		return (1);
	}
	else if (!args[1]) // redirecting to home
		chdir(home->value);
	if (val == -1 && args[1]) // if invalid direction
	{
		perror(args[1]);
	}
	return 0;
}

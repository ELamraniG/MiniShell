#include "../includes/minishell.h"

static int is_allnum(char *s)
{
	int i;

	i = 0;
	while (s[i])
	{
		if (!(s[i] >= '0' && s[i] <= '9'))
			return (0);
		i++;
	}
	return (1);

}

int	exit_shell(char **args)
{
	int	exit_code;

	exit_code = 5222;
	if (args[1])
	{
		if (args[2])
		{
			ft_putstr_fd(2, "minishell: exit: too many arguments\n");
			return (1);
		}
		if (!s_allnum(args[1]))
		{
			ft_putstr_fd(2, "bash: exit: ");
			ft_putstr_fd(2, args[1]);
			ft_putstr_fd(2, ": numeric argument required\n");
			return (1);
		}
		exit(155);
	}
	ft_putstr_fd(1, "exit\n");
	if (!args[1])
		exit(0); 
	exit(exit_code);
	return (exit_code);
}

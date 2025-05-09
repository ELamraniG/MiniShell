#include "../includes/minishell.h"

int	exit_shell(char **args)
{
	int	exit_code;
    exit_code = 5222;
	ft_putstr_fd(1, "exit\n");
	if (!args[1])
		exit(0); 
	// exit_code = ft_atoi(args[1]);
	exit(exit_code);
	return (exit_code);
}

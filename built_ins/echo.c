#include "../includes/minishell.h"

static int	kayna_n(char *s)
{
	int	i;

	if (!s || s[0] != '-')
		return (0);
	i = 1;
	while (s[i])
	{
		if (s[i] != 'n')
			return (0);
		i++;
	}
	if (i == 1) 
		return (0);
	return (1);
}

int	echo(char **args)
{
	int	i;
	int	n_flag;

	if (!args)
		return (1);
	n_flag = 0;
	i = 1;
	while (args[i] && kayna_n(args[i]))
	{
		n_flag = 1;
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		i++;
		if (args[i])
			printf(" ");
	}
	if (!n_flag)
		printf("\n");
	return (0);
}

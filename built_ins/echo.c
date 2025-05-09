#include "../includes/minishell.h"

int	kayna_n(char *s)
{
	int		i;

	i = 0;
	if (s[i] != '-')
		return (0);
	i++;
	while (s[i])
	{
		if (s[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	arg_counter(char **d)
{
	int	i;

	i = 0;
	if (!d)
		return (0);
	while (d[i])
		i++;
	return (i);
}

int	echo(char **args)
{
	int			flag;
	int			count;
	int			i;

	count = arg_counter(args);
	flag = 0;
	i = 1;
	if (!args)
		return (1); 
	if (args[1] == NULL) // for when we use echo alone
	{
		printf("\n");
		return (0); 
	}
	while (args[i] && kayna_n(args[i])) // checking if we have -n and -nnnnn and skipping them
	{
		i++;
		flag = 1;
	}
	while (args[i]) // printing everything after echo
	{
		printf("%s", args[i]);
		count--;
		if (count > 0)
			printf(" ");
		i++;
	}
	if (!flag)
		printf("\n");
	return (0); 
}

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

void	echo(t_ast_tree *ast)
{
	int			flag;
	int			count;
	int			i;

	count = arg_counter(ast->args);
	flag = 0;
	i = 1;
	if (!ast)
		return ;
	if (ast->args[1] == NULL) // for when we use echo alone
	{
		printf("\n");
		return ;
	}
	while (ast->args[i] && kayna_n(ast->args[i])) // checking if we have -n and -nnnnn and skipping them
	{
		i++;
		flag = 1;
	}
	while (ast->args[i]) // printing everything after echo
	{
		printf("%s", ast->args[i]);
		count--;
		if (count > 0)
			printf(" ");
		i++;
	}

	if (!flag)
		printf("\n");
	// printf("%d", flag);

}

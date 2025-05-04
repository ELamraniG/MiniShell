#include "../includes/minishell.h"

int	kayna_n(t_lex_list *d)
{
	t_lex_list *tmp;
	int			i;

	tmp = d;
	i = 0;
	if (tmp->s[i] != '-')
		return (0);
	i++;
	while (tmp->s[i])
	{
		if (tmp->s[i] != 'n')
			return (0);
		i++;
	}
	return (1);

}

void	echo(t_lex_list *d)
{
	t_lex_list	*tmp;
	int			flag;

	flag = 0;
	tmp = d;
	if (!tmp)
		return ;
	if (!ft_strcmp(tmp->s, "echo"))
	{
		if (tmp->next == NULL)
		{
			printf("\n");
			return ;
		}
		tmp = tmp->next;
		while (tmp && kayna_n(tmp))
		{
			tmp = tmp->next;
			flag = 1;
		}
		while (tmp)
		{
			printf("%s", tmp->s);
			tmp = tmp->next;
		}
	}
	if (!flag)
		printf("\n");
}
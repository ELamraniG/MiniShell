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

int	arg_counter(t_lex_list *d)
{
	t_lex_list	*p;
	int			count;

	p = d;
	count = 0;
	while (p)
	{
		count++;
		p = p->next;
	}
	return (count);
}

void	echo(t_lex_list *d)
{
	t_lex_list	*tmp;
	int			flag;
	int			count;

	flag = 0;
	tmp = d;
	count = arg_counter(tmp);
	if (!tmp)
		return ;
	if (!ft_strcmp(tmp->s, "echo"))
	{
		if (tmp->next == NULL) // for when we use echo alone
		{
			printf("\n");
			return ;
		}
		tmp = tmp->next; // to skip echo
		while (tmp && kayna_n(tmp)) // checking if we have -n and -nnnnn and skipping them
		{
			tmp = tmp->next;
			flag = 1;
		}
		while (tmp) // printing everything after echo
		{
			printf("%s", tmp->s);
			count--;
			if (count > 0)
				printf(" ");
			tmp = tmp->next;
		}
	}
	if (!flag)
		printf("\n");
}

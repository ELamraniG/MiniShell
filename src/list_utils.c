#include "../includes/minishell.h"

t_lexical_list	*new_node(char *str)
{
	t_lexical_list	*p;

	p = malloc(sizeof(t_lexical_list));
	if (!p)
		return (NULL);
	p->s = str;
	p->next = NULL;
	return (p);
}

void	add_to_list(t_lexical_list **ll, char *str)
{
	if (!ll)
		return;
	t_lexical_list *tmp = *ll; 
	while (tmp && tmp->next)
		tmp = (tmp)->next;
	t_lexical_list *t = new_node(str);
	if (!t)
		return;
	if (!tmp)
	{
		*ll = t;
		return ;
	}
	(tmp)->next = t;
}
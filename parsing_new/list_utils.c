# include "../includes/minishell_new.h"

t_lex_list	*new_node(char *str, t_q_flags quote, int is_space)
{
	t_lex_list	*p;

	p = malloc(sizeof(t_lex_list));
	if (!p)
		return (NULL);
	p->s = str;
	p->q_type = quote;
	p->is_space = is_space;
	p->next = NULL;
	return (p);
}


void	add_to_list(t_lex_list **ll, char *str, t_q_flags quote, int is_space)
{
	if (!ll)
		return ;
	t_lex_list *tmp = *ll;
	while (tmp && tmp->next)
		tmp = (tmp)->next;
	t_lex_list *t = new_node(str, quote, is_space);
	if (!t)
		return ;
	if (!tmp)
	{
		*ll = t;
		return ;
	}
	(tmp)->next = t;
}

t_redirect	*new_node_redir(char *str, int type)
{
	t_redirect	*p;

	p = malloc(sizeof(t_redirect));
	if (!p)
		return (NULL);
	p->file_name = str;
	p->type = type;
	p->next = NULL;
	return (p);
}


void	add_to_list_redir(t_redirect **ll, char *str, int type)
{
	if (!ll)
		return ;
	t_redirect *tmp = *ll;
	while (tmp && tmp->next)
		tmp = (tmp)->next;
	t_redirect *t = new_node_redir(str, type);
	if (!t)
		return ;
	if (!tmp)
	{
		*ll = t;
		return ;
	}
	(tmp)->next = t;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:15:11 by moel-amr          #+#    #+#             */
/*   Updated: 2025/05/04 22:22:47 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
	t_lex_list	*tmp;
	t_lex_list	*t;

	if (!ll)
		return ;
	tmp = *ll;
	while (tmp && tmp->next)
		tmp = (tmp)->next;
	t = new_node(str, quote, is_space);
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
	t_redirect	*tmp;
	t_redirect	*t;

	if (!ll)
		return ;
	tmp = *ll;
	while (tmp && tmp->next)
		tmp = (tmp)->next;
	t = new_node_redir(str, type);
	if (!t)
		return ;
	if (!tmp)
	{
		*ll = t;
		return ;
	}
	(tmp)->next = t;
}

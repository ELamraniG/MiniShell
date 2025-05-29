/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:15:11 by moel-amr          #+#    #+#             */
/*   Updated: 2025/05/29 23:12:02 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minishell_bonus.h"

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

void	add_to_list_redir(t_redirect **ll, t_redirect *t)
{
	t_redirect	*tmp;

	if (!ll)
		return ;
	tmp = *ll;
	while (tmp && tmp->next)
		tmp = (tmp)->next;
	if (!t)
		return ;
	if (!tmp)
	{
		*ll = t;
		return ;
	}
	(tmp)->next = t;
}

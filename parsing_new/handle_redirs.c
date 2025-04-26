/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-amr <moel-amr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:16:40 by moel-amr          #+#    #+#             */
/*   Updated: 2025/04/26 16:18:33 by moel-amr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_special_for_redirs(int a)
{
	if (a == WORD || a < 4)
		return (1);
	return (0);
}

static int	calculate_words(t_lex_list *token)
{
	int	n;

	n = 0;
	while (token && is_special_for_redirs(token->a_type))
	{
		if (token->a_type == WORD)
			n++;
		if (token->a_type > 3)
			token = token->next;
		else if (token->a_type < 4)
			token = token->next->next;
	}
	return (n);
}

void	handle_words(t_ast_tree *node, t_lex_list **token)
{
	char		**args;
	int			malc;
	t_redirect	*tmp;
	int			i;

	i = 0;
	tmp = NULL;
	malc = calculate_words(*token);
	args = malloc(sizeof(char *) * (malc + 1));
	args[malc] = 0;
	while (*token && is_special_for_redirs((*token)->a_type))
	{
		if ((*token)->a_type == WORD)
		{
			args[i++] = ft_strdup((*token)->s);
			(*token) = (*token)->next;
		}
		else if ((*token)->a_type < 4)
		{
			add_to_list_redir(&tmp, (*token)->next->s, (*token)->a_type);
			(*token) = (*token)->next->next;
		}
	}
	node->args = args;
	node->redirect = tmp;
}

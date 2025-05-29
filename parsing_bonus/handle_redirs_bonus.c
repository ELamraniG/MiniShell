/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirs_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:16:40 by moel-amr          #+#    #+#             */
/*   Updated: 2025/05/29 23:11:36 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minishell_bonus.h"

static void	add_to_list_redirss(t_redirect **ll, int type, t_lex_list **token)
{
	t_redirect	*new_node;
	t_redirect	*tmp;

	new_node = malloc(sizeof(t_redirect));
	new_node->type = type;
	new_node->next = NULL;
	new_node->heredoc = -1;
	new_node->final_file_name = NULL;
	redir_files(token, new_node);
	if (*ll == NULL)
		*ll = new_node;
	else
	{
		tmp = *ll;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_node;
	}
}

static void	handle_word_norm(t_h_word *hword, t_lex_list *token, t_ast_tree *n)
{
	hword->q_type = NULL;
	hword->is_space = NULL;
	hword->i = 0;
	hword->getting_redirs = NULL;
	hword->malc = calculate_words(token);
	hword->args = malloc(sizeof(char *) * (hword->malc + 1));
	hword->q_type = malloc(sizeof(int) * (hword->malc));
	hword->is_space = malloc(sizeof(int) * (hword->malc));
	n->arg_counter = hword->malc;
	hword->args[hword->malc] = 0;
}

void	handle_words(t_ast_tree *node, t_lex_list **token)
{
	t_h_word	data;

	handle_word_norm(&data, *token, node);
	while (*token && is_special_for_redirs((*token)->a_type))
	{
		if ((*token)->a_type == WORD)
		{
			data.args[data.i] = ft_strdup((*token)->s);
			data.q_type[data.i] = (*token)->q_type;
			data.is_space[data.i] = (*token)->is_space;
			data.i++;
			(*token) = (*token)->next;
		}
		else if ((*token)->a_type < 4)
		{
			data.r_type = (*token)->a_type;
			(*token) = (*token)->next;
			add_to_list_redirss(&data.getting_redirs, data.r_type, token);
		}
	}
	node->is_space = data.is_space;
	node->q_type = data.q_type;
	node->args = data.args;
	node->redirect = data.getting_redirs;
}

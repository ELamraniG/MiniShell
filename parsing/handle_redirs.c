/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-amr <moel-amr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:16:40 by moel-amr          #+#    #+#             */
/*   Updated: 2025/05/20 13:25:40 by moel-amr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_special_for_redirs(int a)
{
	if (a == WORD || a < 4)
		return (1);
	return (0);
}

static int	wach_skip(t_lex_list *token)
{
	if (!token || !token->next)
		return (0);
	if (!token->is_space && token->next->a_type == WORD)
		return (1);
	return (0);
}

static int	calculate_words(t_lex_list *token)
{
	int			n;
	t_lex_list	*t;

	n = 0;
	while (token && is_special_for_redirs(token->a_type))
	{
		if (token->a_type == WORD)
		{
			n++;
			token = token->next;
		}
		else if (token->a_type < 4)
		{
			token = token->next;
			t = token;
			token = token->next;
			while (wach_skip(t))
			{
				t = token;
				token = token->next;
			}
		}
	}
	return (n);
}

static int	count_files(t_lex_list *token)
{
	int			count;
	t_lex_list	*current;

	count = 1;
	current = token;
	while (wach_skip(current))
	{
		count++;
		current = current->next;
	}
	return (count);
}

static void	redir_files(t_lex_list **token, t_redirect *redir)
{
	int			i;
	t_lex_list	*current;
	int			count;

	i = 0;
	count = count_files(*token);
	redir->file_str_count = count;
	redir->file_name = malloc(sizeof(char *) * count);
	redir->q_types = malloc(sizeof(int) * count);
	redir->is_space = malloc(sizeof(int) * count);
	redir->file_name[i] = ft_strdup((*token)->s);
	redir->q_types[i] = (*token)->q_type;
	redir->is_space[i] = (*token)->is_space;
	i++;
	current = *token;
	*token = (*token)->next;
	while (i < count)
	{
		redir->file_name[i] = ft_strdup((*token)->s);
		redir->q_types[i] = (*token)->q_type;
		redir->is_space[i] = (*token)->is_space;
		i++;
		current = *token;
		*token = (*token)->next;
	}
}

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

static void	handle_word_norm(t_h_word *hword, t_lex_list *token, t_ast_tree *node)
{
	hword->q_type = NULL;
	hword->is_space = NULL;
	hword->i = 0;
	hword->getting_redirs = NULL;
	hword->malc = calculate_words(token);
	hword->args = malloc(sizeof(char *) * (hword->malc + 1));
	hword->q_type = malloc(sizeof(int) * (hword->malc));
	hword->is_space = malloc(sizeof(int) * (hword->malc));
	node->arg_counter = hword->malc;
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

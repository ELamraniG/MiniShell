/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-amr <moel-amr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:16:40 by moel-amr          #+#    #+#             */
/*   Updated: 2025/05/13 14:56:01 by moel-amr         ###   ########.fr       */
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

static int	get_file_count(t_lex_list *token)
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

static void	collect_redirect_parts(t_lex_list **token, t_redirect *redir)
{
	int			i;
	t_lex_list	*current;
	int			file_count;

	i = 0;
	file_count = get_file_count(*token);
	redir->file_str_count = file_count;
	redir->file_name = malloc(sizeof(char *) * file_count);
	redir->q_types = malloc(sizeof(int) * file_count);
	redir->is_space = malloc(sizeof(int) * file_count);
	redir->file_name[i] = ft_strdup((*token)->s);
	redir->q_types[i] = (*token)->q_type;
	redir->is_space[i] = (*token)->is_space;
	i++;
	current = *token;
	*token = (*token)->next;
	while (i < file_count)
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
	new_node->heredoc = 0;
	new_node->LAST_DAMN_FILE_NAME = NULL;
	collect_redirect_parts(token, new_node);
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

void	handle_words(t_ast_tree *node, t_lex_list **token)
{
	char		**args;
	int			malc;
	t_redirect	*getting_redirs;
	int			i;
	int			*q_type;
	int			*is_space;
	int			r_type;

	q_type = NULL;
	is_space = NULL;
	i = 0;
	getting_redirs = NULL;
	malc = calculate_words(*token);
	args = malloc(sizeof(char *) * (malc + 1));
	q_type = malloc(sizeof(int) * (malc));
	is_space = malloc(sizeof(int) * (malc));
	node->arg_counter = malc;
	args[malc] = 0;
	while (*token && is_special_for_redirs((*token)->a_type))
	{
		if ((*token)->a_type == WORD)
		{
			args[i] = ft_strdup((*token)->s);
			q_type[i] = (*token)->q_type;
			is_space[i] = (*token)->is_space;
			i++;
			(*token) = (*token)->next;
		}
		else if ((*token)->a_type < 4)
		{
			r_type = (*token)->a_type;
			(*token) = (*token)->next;
			add_to_list_redirss(&getting_redirs, r_type, token);
		}
	}
	node->is_space = is_space;
	node->q_type = q_type;
	node->args = args;
	node->redirect = getting_redirs;
}

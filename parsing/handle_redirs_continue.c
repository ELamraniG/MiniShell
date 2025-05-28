/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirs_continue.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 21:48:13 by jhamdaou          #+#    #+#             */
/*   Updated: 2025/05/28 21:48:21 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_special_for_redirs(int a)
{
	if (a == WORD || a < 4)
		return (1);
	return (0);
}

int	wach_skip(t_lex_list *token)
{
	if (!token || !token->next)
		return (0);
	if (!token->is_space && token->next->a_type == WORD)
		return (1);
	return (0);
}

int	calculate_words(t_lex_list *token)
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

int	count_files(t_lex_list *token)
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

void	redir_files(t_lex_list **token, t_redirect *redir)
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

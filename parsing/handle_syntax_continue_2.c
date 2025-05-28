/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_syntax_continue_2.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 21:26:18 by jhamdaou          #+#    #+#             */
/*   Updated: 2025/05/28 21:26:21 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


int	wach_valid_tokens(t_lex_list *current, int *status)
{
	if ((current->a_type == PIPE && current->next
			&& (is_and_or(current->next->a_type)))
		|| (is_and_or(current->a_type) && current->next
			&& (current->next->a_type == PIPE)))
	{
		print_syntax(current->next, status);
		return (0);
	}
	return (1);
}

int	check_empty_parentheses(t_lex_list *token, int *status)
{
	if (token->a_type == OP_PAREN && token->next 
		&& token->next->a_type == CL_PAREN)
	{
		print_syntax(token->next, status);
		return (0);
	}
	return (1);
}

int	word_paren(t_lex_list *token, int *status)
{
	if (token->a_type == WORD && token->next 
		&& token->next->a_type == OP_PAREN)
	{
		print_syntax(token->next, status);
		return (0);
	}
	return (1);
}

int	check_operator_before_close_paren(t_lex_list *token, int *status)
{
	if ((token->a_type == PIPE || is_and_or(token->a_type))
		&& token->next && token->next->a_type == CL_PAREN)
	{
		print_syntax(token, status);
		return (0);
	}
	return (1);
}

int	check_pipe_after_open_paren(t_lex_list *token, int *status)
{
	if (token->a_type == OP_PAREN && token->next 
		&& token->next->a_type == PIPE)
	{
		print_syntax(token->next, status);
		return (0);
	}
	return (1);
}

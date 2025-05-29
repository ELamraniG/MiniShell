/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_syntax_continue_3.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 21:26:13 by jhamdaou          #+#    #+#             */
/*   Updated: 2025/05/28 21:26:20 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_parenth_problm(t_lex_list *token, int *status)
{
	if (token->a_type == CL_PAREN && token->next 
		&& token->next->a_type == OP_PAREN)
	{
		print_syntax(token->next, status);
		return (0);
	}
	return (1);
}

int	check_close_paren_word(t_lex_list *token, int *status)
{
	if (token->a_type == CL_PAREN && token->next 
		&& token->next->a_type == WORD)
	{
		print_syntax(token->next, status);
		return (0);
	}
	return (1);
}

int	validate_parentheses(t_lex_list *token, int *status)
{
	if (!check_empty_parentheses(token, status))
		return (0);
	if (!word_paren(token, status))
		return (0);
	if (!check_operator_before_close_paren(token, status))
		return (0);
	if (!check_pipe_after_open_paren(token, status))
		return (0);
	if (!check_parenth_problm(token, status))
		return (0);
	if (!check_close_paren_word(token, status))
		return (0);
	return (1);
}

int	check_redirs(t_lex_list *token, int *status)
{
	if (is_redirection(token->a_type))
	{
		if (!token->next)
		{
			print_syntax(NULL, status);
			return (0);
		}
		else if (token->next->a_type != WORD)
		{
			print_syntax(token->next, status);
			return (0);
		}
	}
	return (1);
}

int	chec_pipe(t_lex_list *current, int *status)
{
	if (!current->next)
	{
		print_syntax(NULL, status);
		return (0);
	}
	if (current->next->a_type == PIPE)
	{
		print_syntax(current->next, status);
		return (0);
	}
	return (1);
}

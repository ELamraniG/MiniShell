/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_syntax.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:16:46 by moel-amr          #+#    #+#             */
/*   Updated: 2025/05/28 21:26:09 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_and_or(t_lex_list *current, int *status)
{
	if (!current->next)
	{
		print_syntax(NULL, status);
		return (0);
	}
	if (current->next->a_type == PIPE || current->next->a_type == AND
		|| current->next->a_type == OR)
	{
		print_syntax(current->next, status);
		return (0);
	}
	return (1);
}

int	check_token(t_lex_list *current, int *status)
{
	if (is_redirection(current->a_type) && !is_redir_good(current,
			status))
		return (0);
	if (!validate_parentheses(current, status))
		return (0);
	if (current->a_type == PIPE && !chec_pipe(current, status))
		return (0);
	if (is_and_or(current->a_type)
		&& !check_and_or(current, status))
		return (0);
	if (current->next && !wach_valid_tokens(current, status))
		return (0);
	return (1);
}

int	check_syntax(t_lex_list *token, int *status)
{
	if (!check_paren(token))
	{
		print_syntax(NULL, status);
		return (0);
	}
	if (token->a_type == PIPE || token->a_type == AND || token->a_type == OR)
	{
		print_syntax(token, status);
		return (0);
	}
	if (is_redirection(token->a_type) && !check_redirs(token,
			status))
		return (0);
	return (1);
}

int	handle_syntax_errors(t_lex_list *token, int *status)
{
	t_lex_list	*current;

	if (!token)
		return (1);
	if (!check_syntax(token, status))
		return (0);
	current = token;
	while (current)
	{
		if (!check_token(current, status))
			return (0);
		current = current->next;
	}
	return (1);
}

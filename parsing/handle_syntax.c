/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_syntax.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-amr <moel-amr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:16:46 by moel-amr          #+#    #+#             */
/*   Updated: 2025/05/06 14:45:19 by moel-amr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_redirection(t_type_arg type)
{
	return (type == IN_REDIR || type == OUT_REDIR || type == APPEND
		|| type == HEREDOC);
}

static int	is_logical_operator(t_type_arg type)
{
	return (type == AND || type == OR);
}

static int	check_valid_parentheses(t_lex_list *token)
{
	int	p_ct;

	p_ct = 0;
	while (token)
	{
		if (token->a_type == OP_PAREN)
			p_ct++;
		else if (token->a_type == CL_PAREN)
		{
			p_ct--;
			if (p_ct < 0)
				return (0);
		}
		token = token->next;
	}
	return (p_ct == 0);
}

static void	syntax_error(t_lex_list *token, int *status)
{
	ft_putstr_fd(STDERR_FILENO,
		"minishell: syntax error near unexpected token `");
	if (!token)
		ft_putstr_fd(STDERR_FILENO, "newline");
	else if (token->a_type == PIPE)
		ft_putstr_fd(STDERR_FILENO, "|");
	else if (token->a_type == AND)
		ft_putstr_fd(STDERR_FILENO, "&&");
	else if (token->a_type == OR)
		ft_putstr_fd(STDERR_FILENO, "||");
	else if (token->a_type == OP_PAREN)
		ft_putstr_fd(STDERR_FILENO, "(");
	else if (token->a_type == CL_PAREN)
		ft_putstr_fd(STDERR_FILENO, ")");
	else if (token->s)
		ft_putstr_fd(STDERR_FILENO, token->s);
	ft_putstr_fd(STDERR_FILENO, "'\n");
	*status = 2;
}

static int	validate_redirection(t_lex_list *token, int *status)
{
		t_lex_list *error_token;

	if (!token->next || token->next->a_type != WORD)
	{
		if (token->next)
			error_token = token->next;
		else
			error_token = NULL;
		syntax_error(error_token, status);
		return (0);
	}
	return (1);
}

static int	wach_valid_tokens(t_lex_list *current, int *status)
{
	if ((current->a_type == PIPE && current->next
			&& (is_logical_operator(current->next->a_type)))
		|| (is_logical_operator(current->a_type) && current->next
			&& (current->next->a_type == PIPE)))
	{
		syntax_error(current->next, status);
		return (0);
	}
	return (1);
}

static int	validate_parentheses(t_lex_list *token, int *status)
{
	t_lex_list	*current;
	int			n;

	if (token->a_type == OP_PAREN && token->next
		&& token->next->a_type == CL_PAREN)
	{
		syntax_error(token->next, status);
		return (0);
	}
	if (token->a_type == WORD && token->next && token->next->a_type == OP_PAREN)
	{
		syntax_error(token->next, status);
		return (0);
	}
	if (token->a_type == OP_PAREN)
	{
		current = token->next;
		n = 1;
		while (current && n > 0)
		{
			if (current->a_type == OP_PAREN)
				n++;
			else if (current->a_type == CL_PAREN)
			{
				n--;
			}
			current = current->next;
		}
		current = token->next;
		n = 1;
		while (current && n > 0)
		{
			if (current->a_type == OP_PAREN)
				n++;
			else if (current->a_type == CL_PAREN)
				n--;
			if (n > 0 && current->next && !wach_valid_tokens(current,
					status))
				return (0);
			if (n == 1 && current->a_type == PIPE && current->next
				&& current->next->a_type == CL_PAREN)
			{
				syntax_error(current, status);
				return (0);
			}
			if (n == 1 && token->next == current && current->a_type == PIPE)
			{
				syntax_error(current, status);
				return (0);
			}
			if (n == 1 && is_logical_operator(current->a_type)
				&& current->next && current->next->a_type == CL_PAREN)
			{
				syntax_error(current, status);
				return (0);
			}
			current = current->next;
		}
	}
	return (1);
}

static int	check_initial_redirection(t_lex_list *token, int *status)
{
	if (is_redirection(token->a_type))
	{
		if (!token->next)
		{
			syntax_error(NULL, status);
			return (0);
		}
		else if (token->next->a_type != WORD)
		{
			syntax_error(token->next, status);
			return (0);
		}
	}
	return (1);
}

int	handle_syntax_errors(t_lex_list *token, int *status)
{
	t_lex_list	*current;

	if (!token)
		return (1);
	if (!check_valid_parentheses(token))
	{
		syntax_error(NULL, status);
		return (0);
	}
	if (token->a_type == PIPE || token->a_type == AND || token->a_type == OR)
	{
		syntax_error(token, status);
		return (0);
	}
	if (is_redirection(token->a_type) && !check_initial_redirection(token,
			status))
		return (0);
	current = token;
	while (current)
	{
		if (is_redirection(current->a_type) && !validate_redirection(current,
				status))
			return (0);
		if (!validate_parentheses(current, status))
			return (0);
		if (current->a_type == PIPE)
		{
			if (!current->next)
			{
				syntax_error(NULL, status);
				return (0);
			}
			if (current->next->a_type == PIPE)
			{
				syntax_error(current->next, status);
				return (0);
			}
			if (current->next && is_redirection(current->next->a_type))
			{
				syntax_error(current->next, status);
				return (0);
			}
		}
		if (is_logical_operator(current->a_type))
		{
			if (!current->next)
			{
				syntax_error(NULL, status);
				return (0);
			}
			if (current->next->a_type == PIPE || current->next->a_type == AND
				|| current->next->a_type == OR)
			{
				syntax_error(current->next, status);
				return (0);
			}
		}
		if (current->next && !wach_valid_tokens(current, status))
			return (0);
		current = current->next;
	}
	return (1);
}

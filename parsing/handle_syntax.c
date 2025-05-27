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

static int	is_and_or(t_type_arg type)
{
	return (type == AND || type == OR);
}

static int	check_paren(t_lex_list *token)
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

static void	print_syntax(t_lex_list *token, int *status)
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

static int	is_redir_good(t_lex_list *token, int *status)
{
		t_lex_list *error_token;

	if (!token->next || token->next->a_type != WORD)
	{
		if (token->next)
			error_token = token->next;
		else
			error_token = NULL;
		print_syntax(error_token, status);
		return (0);
	}
	return (1);
}

static int	wach_valid_tokens(t_lex_list *current, int *status)
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

static int	check_empty_parentheses(t_lex_list *token, int *status)
{
	if (token->a_type == OP_PAREN && token->next 
		&& token->next->a_type == CL_PAREN)
	{
		print_syntax(token->next, status);
		return (0);
	}
	return (1);
}

static int	word_paren(t_lex_list *token, int *status)
{
	if (token->a_type == WORD && token->next 
		&& token->next->a_type == OP_PAREN)
	{
		print_syntax(token->next, status);
		return (0);
	}
	return (1);
}

static int	check_operator_before_close_paren(t_lex_list *token, int *status)
{
	if ((token->a_type == PIPE || is_and_or(token->a_type))
		&& token->next && token->next->a_type == CL_PAREN)
	{
		print_syntax(token, status);
		return (0);
	}
	return (1);
}

static int	check_pipe_after_open_paren(t_lex_list *token, int *status)
{
	if (token->a_type == OP_PAREN && token->next 
		&& token->next->a_type == PIPE)
	{
		print_syntax(token->next, status);
		return (0);
	}
	return (1);
}

static int	check_parenth_problm(t_lex_list *token, int *status)
{
	if (token->a_type == CL_PAREN && token->next 
		&& token->next->a_type == OP_PAREN)
	{
		print_syntax(token->next, status);
		return (0);
	}
	return (1);
}

static int	check_close_paren_word(t_lex_list *token, int *status)
{
	if (token->a_type == CL_PAREN && token->next 
		&& token->next->a_type == WORD)
	{
		print_syntax(token->next, status);
		return (0);
	}
	return (1);
}

static int	validate_parentheses(t_lex_list *token, int *status)
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

static int	check_redirs(t_lex_list *token, int *status)
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

static int	chec_pipe(t_lex_list *current, int *status)
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

static int	check_and_or(t_lex_list *current, int *status)
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

static int	check_token(t_lex_list *current, int *status)
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

static int	check_syntax(t_lex_list *token, int *status)
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

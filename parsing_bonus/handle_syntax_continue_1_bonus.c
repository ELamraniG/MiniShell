/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_syntax_continue_1_bonus.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 21:26:21 by jhamdaou          #+#    #+#             */
/*   Updated: 2025/05/29 23:11:44 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minishell_bonus.h"

int	is_redirection(t_type_arg type)
{
	return (type == IN_REDIR || type == OUT_REDIR || type == APPEND
		|| type == HEREDOC);
}

int	is_and_or(t_type_arg type)
{
	return (type == AND || type == OR);
}

int	check_paren(t_lex_list *token)
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

void	print_syntax(t_lex_list *token, int *status)
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

int	is_redir_good(t_lex_list *token, int *status)
{
	t_lex_list	*error_token;

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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xminishell_continue_2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 22:43:14 by jhamdaou          #+#    #+#             */
/*   Updated: 2025/05/29 22:43:19 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_token(t_lex_list **tokens, char **input, int *status)
{
	*tokens = lexing_the_thing(*input, status);
	if (!tokens)
	{
		free(*input);
		return (-1);
	}
	set_the_arg_type(*tokens);
	return (0);
}

int	syntax_err_normted(t_lex_list **tokens, int *status, char **input)
{
	if (!handle_syntax_errors(*tokens, status))
	{
		free(*input);
		free_lex_list(*tokens);
		return (0);
	}
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-amr <moel-amr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:14:33 by moel-amr          #+#    #+#             */
/*   Updated: 2025/04/26 16:14:37 by moel-amr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	remove_quotes(t_lex_list *token)
{
	char	*tmp;

	while (token)
	{
		if (token->a_type == WORD && token->q_type != NQ)
		{
			tmp = token->s;
			token->s = ft_substr(token->s, 1, ft_strlen(token->s) - 2);
			free(tmp);
		}
		token = token->next;
	}
}

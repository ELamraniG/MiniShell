/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:14:33 by moel-amr          #+#    #+#             */
/*   Updated: 2025/05/29 23:12:05 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minishell_bonus.h"

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

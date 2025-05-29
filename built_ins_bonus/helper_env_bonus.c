/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:46:48 by jhamdaou          #+#    #+#             */
/*   Updated: 2025/05/26 19:33:59 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_node(t_env_list *current)
{
	free(current->key);
	free(current->value);
	free(current);
}

t_env_list	*find_key_node(t_env_list *current, char *key, t_env_list **prev)
{
	*prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current);
		*prev = current;
		current = current->next;
	}
	return (NULL);
}

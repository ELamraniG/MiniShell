/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:46:48 by jhamdaou          #+#    #+#             */
/*   Updated: 2025/05/29 23:08:08 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minishell_bonus.h"

void	insert_node_first(t_env_list **d, char *key, char *value, int flag)
{
	t_env_list	*tmp;

	tmp = (*d);
	tmp = new_env_node();
	tmp->key = ft_strdup(key);
	tmp->value = ft_strdup(value);
	tmp->flag = flag;
	*d = tmp;
	return ;
}

void	add_last(t_env_list **d, char *key, char *value, int flag)
{
	t_env_list	*tmp;

	tmp = (*d);
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_env_node();
	tmp->next->key = ft_strdup(key);
	tmp->next->value = ft_strdup(value);
	tmp->next->flag = flag;
}

void	insert_node(t_env_list **d, char *key, char *value, int flag)
{
	t_env_list	*tmp;

	if (!d || !key)
		return ;
	tmp = (*d);
	if (!tmp)
	{
		insert_node_first(d, key, value, flag);
		return ;
	}
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, key))
		{
			free(tmp->value);
			tmp->value = ft_strdup(value);
			return ;
		}
		tmp = tmp->next;
	}
	add_last(d, key, value, flag);
}

void	delete_node(t_env_list **head, char *key)
{
	t_env_list	*current;
	t_env_list	*prev;

	if (!key || !*head)
		return ;
	current = find_key_node(*head, key, &prev);
	if (current == (*head))
	{
		(*head) = (*head)->next;
		free_node(current);
		return ;
	}
	if (!current)
		return ;
	if (prev == NULL)
		*head = current->next;
	else
		prev->next = current->next;
	free_node(current);
}

t_env_list	*get_env_value(t_env_list *env_list, char *key)
{
	t_env_list	*tmp;

	tmp = env_list;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, key))
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

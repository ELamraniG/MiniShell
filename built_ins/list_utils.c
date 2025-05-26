/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:46:48 by jhamdaou          #+#    #+#             */
/*   Updated: 2025/05/26 17:46:49 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	insert_node_last(t_env_list **d, char *key, char *value, int flag)
{
	t_env_list	*tmp;

	if (!d || !key)
		return ;
	tmp = (*d);
	if (!tmp) // if first node is empty
	{
		tmp = new_env_node();
		tmp->key = ft_strdup(key);
		tmp->value = ft_strdup(value);
		tmp->flag = flag;
		*d = tmp;
		return ;
	}
	while (tmp) // checking if we already have the key, so we only change the value instead of creating a new node
	{
		if (!ft_strcmp(tmp->key, key))
		{
			free(tmp->value);
			tmp->value = ft_strdup(value);
			return ;
		}
		tmp = tmp->next;
	}
	tmp = (*d);
	while (tmp->next) // iterating to the end to add the node at the end
		tmp = tmp->next;
	tmp->next = new_env_node();
	tmp->next->key = ft_strdup(key);
	tmp->next->value = ft_strdup(value);
	tmp->next->flag = flag;
}

int	list_size(t_env_list *list)
{
	int	count;

	count = 0;
	while (list)
	{
		list = list->next;
		count++;
	}
	return (count);
}

void	delete_node(t_env_list **head, char *key)
{
	t_env_list	*current;
	t_env_list	*prev;

	if (!key || !*head)
		return ;
	current = *head;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			break ;
		prev = current;
		current = current->next;
	}
	if (current == (*head))
	{
		(*head) = (*head)->next;
		free(current->key);
		free(current->value);
		free(current);
		return ;
	}
	if (!current)
		return ;
	if (prev == NULL)
		*head = current->next;
	else
		prev->next = current->next;
	free(current->key);
	free(current->value);
	free(current);
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

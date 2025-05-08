#include "../includes/minishell.h"

void	insert_node(t_env_list **d, char *key, char *value, int flag)
{
	t_env_list	*tmp;

	tmp = (*d);
	if (!tmp)
	{
		tmp = new_env_node();
		tmp->key = key;
		tmp->value = value;
		tmp->flag = flag;
		*d = tmp;
		return	;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_env_node();
	tmp->next->key = key;
	tmp->next->flag = flag;
	tmp->next->value = value;
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

void	delete_node(t_env_list **env_list, char *key)
{
	t_env_list	*tmp;
	t_env_list	*tmp2;
	t_env_list	*to_delete;
	int			i;

	if (!key || !(*env_list))
		return ;
	tmp = *env_list;
	i = 0;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, key))
			break ;
		tmp = tmp->next;
	}
	if (tmp == NULL)
		return ;
	if (tmp == (*env_list)) // if node to remove was first
	{
		(*env_list) = (*env_list)->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
	else if ((tmp->next == NULL) && !ft_strcmp(tmp->key, key)) // if node to remove was last
	{
		tmp2 = *env_list;
		while (tmp2 && tmp2->next != tmp)
			tmp2 = tmp2->next;
		tmp2->next = NULL;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
	else // if node to remove was in the middle
	{
		to_delete = tmp;
		tmp = *env_list;
		while (tmp && tmp->next != to_delete)
			tmp = tmp->next;
		tmp->next = to_delete->next;
		free(to_delete->key);
		free(to_delete->value);
		free(tmp);
	}
}

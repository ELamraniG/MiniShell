#include "../includes/minishell.h"

void	insert_node(t_env_list *d, char *key, char *value)
{
	while (d->next)
		d = d->next;
	d = new_env_node();
	d->key = key;
	d->value = value;
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
	t_env_list	*to_delete;
	int			mark;
	int			i;

	if (!key)
		return ;
	tmp = *env_list;
	mark = 0;
	i = 0;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, key))
			break ;
		tmp = tmp->next;
		mark++;
		printf("mark is: %d\n", mark);
	}
	if (mark == 0) // if node to remove was first
	{
		printf("mark: %d", mark);
		(*env_list) = (*env_list)->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
	else if ((tmp->next == NULL) && !ft_strcmp(tmp->key, key)) // if node to remove was last
	{
		while (i < list_size(tmp))
			tmp = tmp->next;
		free(tmp->key);
		free(tmp->value);
		tmp = *env_list;
		while (i < list_size(tmp) - 1)
			tmp = tmp->next;
		tmp->next = NULL;
		free(tmp);
	}
	else // if node to remove was in the middle
	{
		printf("||||||| test||||||");
		i = 0;
		while (i != mark)
			tmp = tmp->next;
		to_delete = tmp;
		tmp = *env_list;
		i = 0;
		while (i != mark - 1)
			tmp = tmp->next;
		tmp->next = to_delete->next;
		free(to_delete->key);
		free(to_delete->value);
		free(tmp);
	}
}

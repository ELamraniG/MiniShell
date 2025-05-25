#include "../includes/minishell.h"

char	**join_args_without_spaces(t_ast_tree *astree)
{
	char	**args2;
	char	*tmp;

	int i, j, count;
	count = 1;
	i = 0;
	while (i < astree->arg_counter - 1)
	{
		if (astree->is_space[i])
			count++;
		i++;
	}
	if (count == astree->arg_counter)
		return (astree->args);
	args2 = malloc(sizeof(char *) * (count + 1));
	j = 0;
	args2[0] = ft_strdup(astree->args[0]);
	i = 1;
	while (i < astree->arg_counter)
	{
		if (astree->is_space[i - 1])
		{
			j++;
			args2[j] = ft_strdup(astree->args[i]);
		}
		else
		{
			tmp = args2[j];
			args2[j] = ft_strjoin(tmp, astree->args[i]);
			free(tmp);
		}
		i++;
	}
	args2[count] = NULL;
	i = 0;
	while (i < astree->arg_counter)
	{
		free(astree->args[i]);
		i++;
	}
	free(astree->args);
	astree->arg_counter = count;
	return (args2);
}

static char	*join_files_without_spaces(t_redirect *redir)
{
	char	*args2;
	char	*tmp_free;
	int		i;

	args2 = ft_strdup("");
	i = 0;
	while (i < redir->file_str_count)
	{
		tmp_free = args2;
		args2 = ft_strjoin(args2, redir->file_name[i]);
		free(tmp_free);
		i++;
	}
	return (args2);
}
void	join_all_redir_files_without_spaces(t_ast_tree *astree)
{
	t_redirect	*tmp;

	tmp = astree->redirect;
	while (tmp)
	{
		tmp->final_file_name = join_files_without_spaces(tmp);
		tmp = tmp->next;
	}
}
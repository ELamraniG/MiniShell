#include "../includes/minishell.h"

static void	norm_join_args(int *i, int *j, char ***args2, int count)
{
	*args2 = malloc(sizeof(char *) * (count + 1));
	*i = 1;
	*j = 0;
}

void norm_args2(t_ast_tree *astree,int *i,int *j,char ***args2)
{
	char *tmp;
		while (*i < astree->arg_counter)
	{
		if (astree->is_space[*i - 1])
			args2[0][++(*j)] = ft_strdup(astree->args[*i]);
		else
		{
			tmp = args2[0][*j];
			args2[0][*j] = ft_strjoin(tmp, astree->args[*i]);
			free(tmp);
		}
		(*i)++;
	}
}

char	**join_args_without_spaces(t_ast_tree *astree)
{
	char	**args2;
	char	*tmp;
	int		i;
	int		j;
	int		count;

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
	norm_join_args(&i, &j, &args2, count);
	args2[0] = ft_strdup(astree->args[0]);
	norm_args2(astree,&i,&j,&args2);
	args2[count] = NULL;
	i = 0;
	while (i < astree->arg_counter)
		free(astree->args[i++]);
	return (free(astree->args), astree->arg_counter = count, args2);
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
	t_redirect *tmp;

	tmp = astree->redirect;
	while (tmp)
	{
		tmp->final_file_name = join_files_without_spaces(tmp);
		tmp = tmp->next;
	}
}
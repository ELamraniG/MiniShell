#include "includes/minishell.h"

static int	wach_exists(const char *pat, const char *str)
{
	int	sLen;
	int	pLen;
	int	s = 0, p;
	int	star = -1, match;

	sLen = ft_strlen(str);
	pLen = ft_strlen(pat);
	s = 0, p = 0;
	star = -1, match = -1;
	while (s < sLen)
	{
		// Match character
		if (p < pLen && pat[p] == str[s])
		{
			s++;
			p++;
		}
		// Match '*'
		else if (p < pLen && pat[p] == '*')
		{
			star = p;
			match = s;
			p++;
		}
		// Backtrack if previous '*' exists
		else if (star != -1)
		{
			p = star + 1;
			match++;
			s = match;
		}
		// No match
		else
		{
			return (0);
		}
	}
	// Check for remaining characters in pattern
	while (p < pLen && pat[p] == '*')
	{
		p++;
	}
	return (p == pLen);
}

static void	ft_realloc(char ***args, char *s, int *size, int **is_space)
{
	int		i;
	int		*new_is_space;
	char	**new_args;
	int		*tmpint_free;
	char	**tmp_free;

	i = 0;
	new_is_space = malloc(sizeof(int) * (*size + 1));
	new_args = malloc(sizeof(char *) * (*size + 2));
	new_args[*size] = 0;
	while (i < *size)
	{
		new_is_space[i] = is_space[0][i];
		new_args[i] = args[0][i];
		i++;
	}
	new_args[i] = ft_strdup(s);
	new_args[*size + 1] = 0;
	(*size)++;
	tmp_free = *args;
	*args = new_args;
	tmpint_free = is_space[0];
	*is_space = new_is_space;
	free(tmpint_free);
	free(tmp_free);
	return ;
}

static int	it_has_etoil(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '*')
			return (1);
		i++;
	}
	return (0);
}

static void sort_all_dirs(char ***all_dirs, int count)
{
	int i = 0;
	int j = 0;
	char *tmp;
	if (count == 1)
		return;
	while (i < count)
	{
		j = i + 1;
		while (j < count)
		{
			if (ft_strcmp(all_dirs[0][i],all_dirs[0][j]) > 0)
			{
					tmp = all_dirs[0][i];
					all_dirs[0][i] = all_dirs[0][j];
					all_dirs[0][j] = tmp;
			}
			j++;
		}
		i++;
	}
}

static void	handle_single_wild_card(char ***args, char *current_arg,
		int old_is_space, int k, int *size, int **is_space, int q_type)
{
	DIR				*r;
	struct dirent	*reads;
	char			**all_dirs;
	char			*tmp_free;
	int				reads_counter;
	int				j;
	int				i;
	int point_flag = 0;

	all_dirs = NULL;
	reads_counter = 0;
	j = 0;
	i = 0;
	if (current_arg[0] == '.')
		point_flag = 1;
	if (it_has_etoil(current_arg) && q_type == NQ)
	{
		r = opendir(".");
		reads = readdir(r);
		while (reads)
		{
			if (wach_exists(current_arg, reads->d_name))
				reads_counter++;
			reads = readdir(r);
		}
		closedir(r);
		if (reads_counter == 0)
		{
				ft_realloc(args, current_arg, size, is_space);
				is_space[0][*size - 1] = 1;
		}
		else
		{
			r = opendir(".");
			reads = readdir(r);
			all_dirs = malloc(sizeof(char *) * reads_counter);
			while (reads)
			{
				if (wach_exists(current_arg, reads->d_name))
					all_dirs[j++] = reads->d_name;
				reads = readdir(r);
			}
			sort_all_dirs(&all_dirs,reads_counter);
			i = 0;
			while (i < reads_counter)
			{
				if (point_flag == 0 && all_dirs[i][0] == '.')
				{
					i++;
					continue;
				}
				ft_realloc(args, all_dirs[i], size, is_space);
				is_space[0][*size - 1] = 1;
				i++;
			}
			i = 0;
			free(all_dirs);
			closedir(r);
		}
	}
	else
	{
		ft_realloc(args, current_arg, size, is_space);
		is_space[0][*size - 1] = old_is_space;
	}
}

void	handle_file_cards(t_redirect *redir)
{
	int		k;
	char	**args;
	int		size;
	int		*is_space;

	k = 0;
	args = NULL;
	size = 0;
	is_space = NULL;
	k = 0;
	while (redir->file_name[k])
	{
		handle_single_wild_card(&args, redir->file_name[k], redir->is_space[k], k,
			&size, &is_space, redir->q_types[k]);
		k++;
	}
	k = 0;
	while (redir->file_name[k])
		free(redir->file_name[k++]);
	k = 0;
	free(redir->file_name);
	redir->file_name = args;
	redir->file_str_count = size;
	free(redir->is_space);
	redir->is_space = is_space;
	free(redir->q_types);
	redir->q_types = malloc(sizeof(int) * size);
}

int handle_file_wildcard(t_ast_tree *node)
{
	t_redirect *redir = node->redirect;
	while (redir)
	{
		handle_file_cards(redir);
		int i = 0;
		while(i < redir->file_str_count)
			i++;
		if (redir->file_str_count > 1)
		{
			ft_putstr_fd(2,"minishell: ");
			ft_putstr_fd(2,redir->file_name[0]);
			ft_putstr_fd(2,": ambiguous redirect\n");
			return -1;
		}
		redir = redir->next;
	}
	return 1;
}

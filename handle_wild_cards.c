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

static void	ft_realloc(t_wild_norm *wild, char *s)
{
	int		i;
	int		*new_is_space;
	char	**new_args;
	int		*tmpint_free;
	char	**tmp_free;
	int 	*new_q_type;

	i = 0;
	new_is_space = malloc(sizeof(int) * (wild->size + 1));
	new_q_type = malloc(sizeof(int) * (wild->size + 1));
	new_args = malloc(sizeof(char *) * (wild->size + 2));
	new_args[wild->size] = 0;
	while (i < wild->size)
	{
		new_q_type[i] = wild->q_type[i];
		new_is_space[i] = wild->is_space[i];
		new_args[i] = wild->args[i];
		i++;
	}
	new_args[i] = ft_strdup(s);
	new_args[wild->size + 1] = 0;
	(wild->size)++;
	tmp_free = wild->args;
	wild->args = new_args;
	tmpint_free = wild->is_space;
	wild->is_space = new_is_space;
	free(tmpint_free);
	tmpint_free = wild->q_type;
	wild->q_type = new_q_type;
	free(tmpint_free);
	free(tmp_free);
	return ;
}

int	it_has_etoil(char *str)
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

static void	handle_single_wild_card(t_wild_norm *wild, char *current_arg,
		int old_is_space, int old_q_type)
{
	DIR				*r;
	struct dirent	*reads;
	char			**all_dirs;
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
	if (it_has_etoil(current_arg) && old_q_type == NQ)
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
			ft_realloc(wild, current_arg);
			wild->is_space[wild->size - 1] = 1;
			wild->q_type[wild->size - 1] = old_q_type;
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
				ft_realloc(wild, all_dirs[i]);
				wild->is_space[wild->size - 1] = 1;
				wild->q_type[wild->size - 1] = NQ;
				i++;
			}
			i = 0;
			free(all_dirs);
			closedir(r);
		}
	}
	else
	{
		ft_realloc(wild, current_arg);
		wild->is_space[wild->size - 1] = old_is_space;
		wild->q_type[wild->size - 1] = old_q_type;
	}
}

void	handle_wild_card(t_ast_tree *node)
{
	t_wild_norm wild;

	wild.k = 0;
	wild.args = NULL;
	wild.size = 0;
	wild.is_space = NULL;
	wild.q_type = NULL;
	while (node->args[wild.k])
	{
		handle_single_wild_card(&wild, node->args[wild.k], node->is_space[wild.k], node->q_type[wild.k]);
		wild.k++;
	}
	wild.k = 0;
	while (node->args[wild.k])
		free(node->args[wild.k++]);
	// free(node->args[wild.k]);
	free(node->args);
	node->args = wild.args;
	node->arg_counter = wild.size;
	free(node->is_space);
	node->is_space = wild.is_space;
	free(node->q_type);
	node->q_type = wild.q_type;
}

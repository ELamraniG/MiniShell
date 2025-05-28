#include "includes/minishell.h"


static void	free_wild_relc(t_wild_relc *relc)
{
	free(relc->tmpint_free);
	free(relc->tmp_free);
}

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
	t_wild_relc	relc;

	relc.i = 0;
	relc.new_is_space = malloc(sizeof(int) * (wild->size + 1));
	relc.new_q_type = malloc(sizeof(int) * (wild->size + 1));
	relc.new_args = malloc(sizeof(char *) * (wild->size + 2));
	relc.new_args[wild->size] = 0;
	while (relc.i < wild->size)
	{
		relc.new_q_type[relc.i] = wild->q_type[relc.i];
		relc.new_is_space[relc.i] = wild->is_space[relc.i];
		relc.new_args[relc.i] = wild->args[relc.i];
		relc.i++;
	}
	relc.new_args[relc.i] = ft_strdup(s);
	relc.new_args[wild->size + 1] = 0;
	(wild->size)++;
	relc.tmp_free = wild->args;
	wild->args = relc.new_args;
	relc.tmpint_free = wild->is_space;
	wild->is_space = relc.new_is_space;
	free(relc.tmpint_free);
	relc.tmpint_free = wild->q_type;
	wild->q_type = relc.new_q_type;
	return free_wild_relc(&relc);
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

static int	wild_card_norm2(char *current_arg)
{
	DIR				*r;
	struct dirent	*reads;
	int				reads_counter;

	reads_counter = 0;
	r = opendir(".");
	reads = readdir(r);
	while (reads)
	{
		if (wach_exists(current_arg, reads->d_name))
			reads_counter++;
		reads = readdir(r);
	}
	closedir(r);
	return (reads_counter);
}

static char	**wild_card_norm3(char *current_arg, int reads_counter)
{
	DIR				*r;
	struct dirent	*reads;
	char			**all_dirs;
	int				j;

	j = 0;
	r = opendir(".");
	reads = readdir(r);
	all_dirs = malloc(sizeof(char *) * reads_counter);
	while (reads)
	{
		if (wach_exists(current_arg, reads->d_name))
			all_dirs[j++] = reads->d_name;
		reads = readdir(r);
	}
	closedir(r);
	return (all_dirs);
}

static void	wild_card_norm4(t_wild_norm *wild, char **all_dirs, int reads_counter, int point_flag)
{
	int	i;

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
}

static void	wild_card_norm5(t_wild_norm *wild, char *current_arg, int old_is_space, int old_q_type)
{
	ft_realloc(wild, current_arg);
	wild->is_space[wild->size - 1] = old_is_space;
	wild->q_type[wild->size - 1] = old_q_type;
}

static void	wild_card_norm1(t_wild_norm *wild, char *current_arg, int old_q_type, int point_flag)
{
	char	**all_dirs;
	int		reads_counter;

	reads_counter = wild_card_norm2(current_arg);
	if (reads_counter == 0)
	{
		ft_realloc(wild, current_arg);
		wild->is_space[wild->size - 1] = 1;
		wild->q_type[wild->size - 1] = old_q_type;
	}
	else
	{
		all_dirs = wild_card_norm3(current_arg, reads_counter);
		sort_all_dirs(&all_dirs, reads_counter);
		wild_card_norm4(wild, all_dirs, reads_counter, point_flag);
		free(all_dirs);
	}
}

static void	handle_single_wild_card(t_wild_norm *wild, char *current_arg,
		int old_is_space, int old_q_type)
{
	int point_flag = 0;

	if (current_arg[0] == '.')
		point_flag = 1;
	if (it_has_etoil(current_arg) && old_q_type == NQ)
		wild_card_norm1(wild, current_arg, old_q_type, point_flag);
	else
		wild_card_norm5(wild, current_arg, old_is_space, old_q_type);
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
	free(node->args);
	node->args = wild.args;
	node->arg_counter = wild.size;
	free(node->is_space);
	node->is_space = wild.is_space;
	free(node->q_type);
	node->q_type = wild.q_type;
}

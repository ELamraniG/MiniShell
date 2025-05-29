/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_wild_card.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-amr <moel-amr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 21:44:10 by moel-amr          #+#    #+#             */
/*   Updated: 2025/05/29 21:44:10 by moel-amr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	hndl_str_mth(const char *pat, t_exit_norm *sm)
{
	if (sm->p < sm->p_len && pat[sm->p] == '*')
	{
		sm->star = sm->p;
		sm->match = sm->s;
		(sm->p)++;
		return (1);
	}
	else if (sm->star != -1)
	{
		sm->p = sm->star + 1;
		(sm->match)++;
		sm->s = sm->match;
		return (1);
	}
	return (0);
}

static int	wach_exists(const char *pat, const char *str)
{
	t_exit_norm	sm;

	sm.s_len = ft_strlen(str);
	sm.p_len = ft_strlen(pat);
	sm.s = 0;
	sm.p = 0;
	sm.star = -1;
	sm.match = -1;
	while (sm.s < sm.s_len)
	{
		if (sm.p < sm.p_len && pat[sm.p] == str[sm.s])
		{
			sm.s++;
			sm.p++;
		}
		else if (!hndl_str_mth(pat, &sm))
			return (0);
	}
	while (sm.p < sm.p_len && pat[sm.p] == '*')
		sm.p++;
	return (sm.p == sm.p_len);
}

static void	free_file_wild_relc(t_file_wild_relc *relc)
{
	free(relc->tmpint_free);
	free(relc->tmp_free);
}

static void	ft_realloc(t_wild_norm *wild, char *s)
{
	t_file_wild_relc	relc;

	relc.i = 0;
	relc.new_is_space = malloc(sizeof(int) * (wild->size + 1));
	relc.new_q_types = malloc(sizeof(int) * (wild->size + 1));
	relc.new_args = malloc(sizeof(char *) * (wild->size + 2));
	relc.new_args[wild->size] = 0;
	while (relc.i < wild->size)
	{
		relc.new_q_types[relc.i] = wild->q_type[relc.i];
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
	wild->q_type = relc.new_q_types;
	return (free_file_wild_relc(&relc));
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

static void	sort_all_dirs(char ***all_dirs, int count)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	j = 0;
	if (count == 1)
		return ;
	while (i < count)
	{
		j = i + 1;
		while (j < count)
		{
			if (ft_strcmp(all_dirs[0][i], all_dirs[0][j]) > 0)
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

static int	file_wild_norm2(char *current_arg)
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

static char	**file_wild_norm3(char *current_arg, int reads_counter)
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

static void	file_wild_norm4(t_wild_norm *wild, char **all_dirs,
		int reads_counter, int point_flag)
{
	int	i;

	i = 0;
	while (i < reads_counter)
	{
		if (point_flag == 0 && all_dirs[i][0] == '.')
		{
			i++;
			continue ;
		}
		ft_realloc(wild, all_dirs[i]);
		wild->is_space[wild->size - 1] = 1;
		wild->q_type[wild->size - 1] = NQ;
		i++;
	}
}

static void	file_wild_norm5(t_wild_norm *wild, char *current_arg,
		int old_is_space, int old_q_type)
{
	ft_realloc(wild, current_arg);
	wild->is_space[wild->size - 1] = old_is_space;
	wild->q_type[wild->size - 1] = old_q_type;
}

static void	file_wild_norm1(t_wild_norm *wild, char *current_arg,
		int old_q_type, int point_flag)
{
	char	**all_dirs;
	int		reads_counter;

	reads_counter = file_wild_norm2(current_arg);
	if (reads_counter == 0)
	{
		ft_realloc(wild, current_arg);
		wild->is_space[wild->size - 1] = 1;
		wild->q_type[wild->size - 1] = old_q_type;
	}
	else
	{
		all_dirs = file_wild_norm3(current_arg, reads_counter);
		sort_all_dirs(&all_dirs, reads_counter);
		file_wild_norm4(wild, all_dirs, reads_counter, point_flag);
		free(all_dirs);
	}
}

static void	handle_single_wild_card(t_wild_norm *wild, char *current_arg,
		int old_is_space, int old_q_type)
{
	int	point_flag;

	point_flag = 0;
	if (current_arg[0] == '.')
		point_flag = 1;
	if (it_has_etoil(current_arg) && old_q_type == NQ)
		file_wild_norm1(wild, current_arg, old_q_type, point_flag);
	else
		file_wild_norm5(wild, current_arg, old_is_space, old_q_type);
}

void	handle_file_cards(t_redirect *redir)
{
	t_wild_norm	wild;

	wild.k = 0;
	wild.args = NULL;
	wild.size = 0;
	wild.is_space = NULL;
	wild.q_type = NULL;
	while (redir->file_name[wild.k])
	{
		handle_single_wild_card(&wild, redir->file_name[wild.k],
			redir->is_space[wild.k], redir->q_types[wild.k]);
		wild.k++;
	}
	wild.k = 0;
	while (redir->file_name[wild.k])
		free(redir->file_name[wild.k++]);
	free(redir->file_name);
	redir->file_name = wild.args;
	redir->file_str_count = wild.size;
	free(redir->is_space);
	redir->is_space = wild.is_space;
	free(redir->q_types);
	redir->q_types = wild.q_type;
}

static int	check_abg(t_redirect *redir, int old)
{
	int	i;

	i = 0;
	while (i < redir->file_str_count)
		i++;
	if (redir->file_str_count > old)
	{
		ft_putstr_fd(2, "minishell: ");
		ft_putstr_fd(2, redir->file_name[0]);
		ft_putstr_fd(2, ": ambiguous redirect\n");
		return (-1);
	}
	return (1);
}

int	handle_file_wildcard(t_ast_tree *node)
{
	t_redirect	*redir;
	int			old;

	redir = node->redirect;
	while (redir)
	{
		if (redir->type != HEREDOC)
		{
			old = redir->file_str_count;
			handle_file_cards(redir);
			if (check_abg(redir, old) == -1)
				return (-1);
		}
		redir = redir->next;
	}
	return (1);
}

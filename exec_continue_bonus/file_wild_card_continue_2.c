/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_wild_card_continue_2.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 22:36:27 by jhamdaou          #+#    #+#             */
/*   Updated: 2025/05/29 22:36:28 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	sort_all_direction(char ***all_dirs, int count)
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

int	file_wild_norm2(char *current_arg)
{
	DIR				*r;
	struct dirent	*reads;
	int				reads_counter;

	reads_counter = 0;
	r = opendir(".");
	reads = readdir(r);
	while (reads)
	{
		if (wach_exist_card(current_arg, reads->d_name))
			reads_counter++;
		reads = readdir(r);
	}
	closedir(r);
	return (reads_counter);
}

char	**file_wild_norm3(char *current_arg, int reads_counter)
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
		if (wach_exist_card(current_arg, reads->d_name))
			all_dirs[j++] = reads->d_name;
		reads = readdir(r);
	}
	closedir(r);
	return (all_dirs);
}

void	file_wild_norm4(t_wild_norm *wild, char **all_dirs,
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
		ft_realloc_filew(wild, all_dirs[i]);
		wild->is_space[wild->size - 1] = 1;
		wild->q_type[wild->size - 1] = NQ;
		i++;
	}
}

void	file_wild_norm5(t_wild_norm *wild, char *current_arg,
		int old_is_space, int old_q_type)
{
	ft_realloc_filew(wild, current_arg);
	wild->is_space[wild->size - 1] = old_is_space;
	wild->q_type[wild->size - 1] = old_q_type;
}

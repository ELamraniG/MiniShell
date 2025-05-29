/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_wild_cards_continue.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 21:47:11 by jhamdaou          #+#    #+#             */
/*   Updated: 2025/05/29 21:47:12 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	wild_card_norm2(char *current_arg)
{
	DIR				*r;
	struct dirent	*reads;
	int				reads_counter;

	reads_counter = 0;
	r = opendir(".");
	reads = readdir(r);
	while (reads)
	{
		if (wach_exists2(current_arg, reads->d_name))
			reads_counter++;
		reads = readdir(r);
	}
	closedir(r);
	return (reads_counter);
}

char	**wild_card_norm3(char *current_arg, int reads_counter)
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
		if (wach_exists2(current_arg, reads->d_name))
			all_dirs[j++] = reads->d_name;
		reads = readdir(r);
	}
	closedir(r);
	return (all_dirs);
}

void	wild_card_norm4(t_wild_norm *wild, char **all_dirs, int reads_counter, int point_flag)
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
		ft_realloc_wild(wild, all_dirs[i]);
		wild->is_space[wild->size - 1] = 1;
		wild->q_type[wild->size - 1] = NQ;
		i++;
	}
}

void	wild_card_norm5(t_wild_norm *wild, char *current_arg, int old_is_space, int old_q_type)
{
	ft_realloc_wild(wild, current_arg);
	wild->is_space[wild->size - 1] = old_is_space;
	wild->q_type[wild->size - 1] = old_q_type;
}

void	wild_card_norm1(t_wild_norm *wild, char *current_arg, int old_q_type, int point_flag)
{
	char	**all_dirs;
	int		reads_counter;

	reads_counter = wild_card_norm2(current_arg);
	if (reads_counter == 0)
	{
		ft_realloc_wild(wild, current_arg);
		wild->is_space[wild->size - 1] = 1;
		wild->q_type[wild->size - 1] = old_q_type;
	}
	else
	{
		all_dirs = wild_card_norm3(current_arg, reads_counter);
		sort_all_dirs_wild(&all_dirs, reads_counter);
		wild_card_norm4(wild, all_dirs, reads_counter, point_flag);
		free(all_dirs);
	}
}

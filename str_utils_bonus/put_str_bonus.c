/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_str_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 21:46:55 by moel-amr          #+#    #+#             */
/*   Updated: 2025/05/29 23:12:39 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minishell_bonus.h"

void	ft_putstr_fd(int fd, char *str)
{
	if (!str || ft_strlen(str) == 0)
		return ;
	write(fd, str, ft_strlen(str));
}

int	ft_isalpha(char c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

int	ft_isalnum(char c)
{
	return ((c >= '0' && c <= '9') || ft_isalpha(c));
}

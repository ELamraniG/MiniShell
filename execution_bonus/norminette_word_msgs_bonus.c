/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   norminette_word_msgs.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:33:13 by jhamdaou          #+#    #+#             */
/*   Updated: 2025/05/29 16:33:27 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_msg1(char *s)
{
	ft_putstr_fd(2, s);
	ft_putstr_fd(2, ": command not found\n");
	exit(127);
}

void	print_msg2(char *s)
{
	ft_putstr_fd(2, s);
	ft_putstr_fd(2, ": Permission denied\n");
	exit(126);
}

void	print_msg3(char *s)
{
	ft_putstr_fd(2, "minishell: ");
	ft_putstr_fd(2, s);
	ft_putstr_fd(2, ": No such file or directory\n");
	exit(127);
}

void	print_msg4(char *s)
{
	ft_putstr_fd(2, s);
	ft_putstr_fd(2, ": is a directory\n");
	exit(126);
}

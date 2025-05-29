/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:46:41 by jhamdaou          #+#    #+#             */
/*   Updated: 2025/05/29 23:07:57 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minishell_bonus.h"

static int	is_allnum(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (!(s[i] >= '0' && s[i] <= '9'))
			return (0);
		i++;
	}
	return (1);
}

int	exit_shell(char **args, int status)
{
	if (args[1])
	{
		if (!is_allnum(args[1]))
		{
			ft_putstr_fd(2, "bash: exit: ");
			ft_putstr_fd(2, args[1]);
			ft_putstr_fd(2, ": numeric argument required\n");
			exit(255);
		}
		if (args[2])
		{
			ft_putstr_fd(2, "minishell: exit: too many arguments\n");
			return (1);
		}
		ft_putstr_fd(1, "exit\n");
		exit(ft_atoi(args[1]));
	}
	ft_putstr_fd(1, "exit\n");
	exit(status); 
	return (status);
}

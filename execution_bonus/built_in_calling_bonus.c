/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_calling_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 21:05:32 by moel-amr          #+#    #+#             */
/*   Updated: 2025/05/29 23:09:45 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minishell_bonus.h"

int	is_built_in(char *cmd)
{
	char	*built_ins[8];
	int		i;

	built_ins[0] = "cd";
	built_ins[1] = "echo";
	built_ins[2] = "exit";
	built_ins[3] = "env";
	built_ins[4] = "pwd";
	built_ins[5] = "unset";
	built_ins[6] = "export";
	built_ins[7] = NULL;
	i = 0;
	if (!cmd)
		return (0);
	while (built_ins[i])
	{
		if (ft_strcmp(cmd, built_ins[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	execute_built_in(char **args, t_env_list **env, int *status)
{
	if (ft_strcmp(args[0], "cd") == 0)
		return (change_dir(args, *env));
	else if (ft_strcmp(args[0], "echo") == 0)
		return (echo(args));
	else if (ft_strcmp(args[0], "exit") == 0)
		return (exit_shell(args, *status));
	else if (ft_strcmp(args[0], "env") == 0)
		return (print_env(*env));
	else if (ft_strcmp(args[0], "pwd") == 0)
		return (exec_pwd(1));
	else if (ft_strcmp(args[0], "unset") == 0)
		return (exec_unset(env, args));
	else if (ft_strcmp(args[0], "export") == 0)
		return (exec_export(env, args));
	return (1);
}

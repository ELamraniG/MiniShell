/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:46:35 by jhamdaou          #+#    #+#             */
/*   Updated: 2025/05/26 19:26:56 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	change_dir(char **args, t_env_list *env)
{
	int			val;
	char		*old;
	t_env_list	*home;
	t_env_list	*pwd;
	t_env_list	*old_pwd;

	home = get_env_value(env, "HOME");
	pwd = get_env_value(env, "PWD");
	old_pwd = get_env_value(env, "OLDPWD");
	old = getcwd(NULL, 0);
	if (!args || !args[0] || ft_strcmp(args[0], "cd") != 0)
		return (1);
	val = chdir(args[1]);
	if (!args[1] && home == NULL)
		return (ft_putstr_fd(2, "minishell: cd: HOME not set\n"), 1);
	else if (!args[1])
		chdir(home->value);
	if (val == -1 && args[1])
		return (perror(args[1]), 1);
	free(old_pwd->value);
	old_pwd->value = old;
	insert_node(&env, pwd->key, getcwd(NULL, 0), 0);
	return (0);
}

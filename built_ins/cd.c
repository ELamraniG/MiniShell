/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-amr <moel-amr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:46:35 by jhamdaou          #+#    #+#             */
/*   Updated: 2025/05/27 16:26:53 by moel-amr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int    change_dir(char **args, t_env_list *env)
{
    int            val;
    char        *old;
    t_env_list    *home;
    t_env_list    *pwd;
    t_env_list    *old_pwd;

    home = get_env_value(env, "HOME");
    pwd = get_env_value(env, "PWD");
    old_pwd = get_env_value(env, "OLDPWD");

    old = getcwd(NULL, 0);
    if (!args || !args[0] || ft_strcmp(args[0], "cd") != 0)
        return (1);
    if (!args[1] && home == NULL)
        return (ft_putstr_fd(2, "minishell: cd: HOME not set\n"), 1);
    else if (!args[1])
        chdir(home->value);
    val = chdir(args[1]);
    if (val == -1 && args[1])
        return (perror(args[1]), 1);
    insert_node(&env, ft_strdup("OLDPWD"), old, 0);
    insert_node(&env, pwd->key, getcwd(NULL, 0), 0);
    return (0);
}
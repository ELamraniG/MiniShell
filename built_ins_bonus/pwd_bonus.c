/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:46:52 by jhamdaou          #+#    #+#             */
/*   Updated: 2025/05/29 23:08:10 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minishell_bonus.h"

int	exec_pwd(int n)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd && n)
	{
		printf("%s\n", cwd);
		free(cwd);
		return (0); 
	}
	return (1); 
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-amr <moel-amr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:10:39 by moel-amr          #+#    #+#             */
/*   Updated: 2025/05/29 21:46:05 by moel-amr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	foo_sign(const char *s, int *i, int *sign)
{
	if (s[*i] == '-' || s[*i] == '+')
	{
		if (s[*i] == '-')
			*sign = -1;
		(*i)++;
	}
}

int	ft_atoi(const char *s)
{
	long	res;
	int		i;
	int		sign;
	long	tmp;

	i = 0;
	res = 0;
	sign = 1;
	while (s[i] == 32 || (s[i] >= 9 && s[i] <= 13))
		i++;
	foo_sign(s, &i, &sign);
	while (s[i] >= '0' && s[i] <= '9')
	{
		tmp = res;
		res *= 10;
		res += (s[i] - 48) * sign;
		i++;
		if (sign == 1 && (tmp > res))
			return (-1);
		else if (sign == -1 && (tmp < res))
			return (0);
	}
	return (res);
}

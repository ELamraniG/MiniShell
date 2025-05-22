/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-amr <moel-amr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 16:18:28 by moel-amr          #+#    #+#             */
/*   Updated: 2025/05/22 16:07:52 by moel-amr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	dlen;
	size_t	slen;
	size_t	i;

	i = 0;
	slen = ft_strlen(src);
	if (dstsize == 0)
		return (slen);
	dlen = ft_strlen(dst);
	if (dlen >= dstsize)
		return (dstsize + slen);
	else
	{
		while (dstsize - dlen - 1 > i && src[i])
		{
			dst[dlen + i] = src[i];
			i++;
		}
		dst[dlen + i] = 0;
	}
	return (dlen + slen);
}


static char	*ft_alo(char *str, size_t slen)
{
	str = malloc((slen) * sizeof(char));
	if (!str)
		return (NULL);
	return (str);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*str;

	str = NULL;
	if (s1 && s2)
	{
		str = ft_alo(str, (ft_strlen(s1) + ft_strlen(s2) + 1));
		if (!str)
			return (NULL);
		ft_strlcpy(str, s1, ft_strlen(s1) + 1);
		ft_strlcat(str, s2, ft_strlen(s1) + ft_strlen(s2) + 1);
	}
	else if (s1)
	{
		str = ft_alo(str, ft_strlen(s1));
		if (str)
			ft_strlcpy(str, s1, ft_strlen(s1) + 1);
	}
	else if (s2)
	{
		str = ft_alo(str, ft_strlen(s2));
		if (str)
			ft_strlcpy(str, s2, ft_strlen(s2) + 1);
	}
	return (str);
}

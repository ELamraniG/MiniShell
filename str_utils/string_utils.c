/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-amr <moel-amr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:14:22 by moel-amr          #+#    #+#             */
/*   Updated: 2025/05/08 16:30:54 by moel-amr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*ss;

	if (!s)
		return (NULL);
	if (len == 0)
	{
		return (ft_strdup(""));
	}
	if (start >= ft_strlen(s))
		return (ft_strdup(""));
	if (len + start > ft_strlen(s))
		len = ft_strlen(s) - start;
	ss = malloc(sizeof(char) * (len + 1));
	if (!ss)
		return (NULL);
	ft_strlcpy(ss, s + start, len + 1);
	return (ss);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	slen;

	if (!dst || !src)
		return (0);
	i = 0;
	slen = ft_strlen(src);
	if (dstsize == 0)
		return (slen);
	while (i < dstsize - 1 && src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = 0;
	return (slen);
}

char	*ft_strdup(const char *s1)
{
	char	*snew;
	int		slen;

	slen = ft_strlen(s1);
	snew = malloc(sizeof(char) * (slen + 1));
	if (snew == NULL)
		return (NULL);
	ft_strlcpy(snew, s1, slen + 1);
	return (snew);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	if (!s1 || !s2)
		return (0);
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

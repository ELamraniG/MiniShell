/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_for_expand.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-amr <moel-amr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 21:46:19 by moel-amr          #+#    #+#             */
/*   Updated: 2025/05/29 21:46:19 by moel-amr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	do_i_j(int *i, int *k)
{
	(*i)++;
	(*k)++;
}

static void	ft_free(char **s, int i)
{
	while (i >= 0)
	{
		free(s[i]);
		i--;
	}
	free(s);
}

static int	ft_word_count(const char *str, char c)
{
	int	i;
	int	wc;

	wc = 0;
	i = 0;
	while (str[i])
	{
		while (str[i] && (str[i] == c))
			i++;
		if (str[i])
			wc++;
		while (str[i] && !(str[i] == c))
			i++;
	}
	if (wc == 0 && str[0] == ' ')
		return (1);
	return (wc);
}

static int	do_some1_cont(const char *s, char **s2, char c, t_sp_norm *nor)
{
	while (s[nor->i])
	{
		nor->k = 0;
		while (s[nor->i] == c)
			(nor->i)++;
		while (s[nor->i] && (!(s[(nor->i)++] == c) || nor->j == nor->wc - 1))
		{
			nor->k++;
		}
		if (nor->k)
		{
			s2[nor->j] = malloc(sizeof(char) * (nor->k + 1));
			if (!s2[nor->j])
			{
				ft_free(s2, nor->j - 1);
				return (0);
			}
			(nor->j)++;
		}
	}
	return (1);
}

static int	do_someth1(const char *s, char **s2, char c, int wc)
{
	t_sp_norm	nor;

	nor.i = 0;
	nor.j = 0;
	nor.k = 0;
	nor.wc = wc;
	while (s[nor.i] && (s[nor.i] == c))
		do_i_j(&nor.i, &nor.k);
	while (s[nor.i] && (!(s[nor.i] == c) || nor.j == nor.wc - 1))
		do_i_j(&nor.i, &nor.k);
	if (nor.k)
	{
		s2[nor.j] = malloc(sizeof(char) * (nor.k + 1));
		if (!s2[nor.j])
		{
			ft_free(s2, nor.j - 1);
			return (0);
		}
		nor.j++;
	}
	return (do_some1_cont(s, s2, c, &nor));
}

static void	do_someth2_remaining(const char *s, char **s2, char c,
		t_sp_norm *nor)
{
	while (s[nor->i])
	{
		nor->k = 0;
		while (s[nor->i] == c)
			(nor->i)++;
		while (s[nor->i] && (!(s[nor->i] == c) || nor->j == nor->wc - 1))
		{
			s2[nor->j][nor->k] = s[nor->i];
			(nor->i)++;
			nor->k++;
		}
		if (nor->k)
		{
			s2[nor->j][nor->k] = 0;
			(nor->j)++;
		}
	}
}

static void	do_someth2(const char *s, char **s2, char c, int wc)
{
	t_sp_norm	nor;

	nor.i = 0;
	nor.j = 0;
	nor.k = 0;
	nor.wc = wc;
	while (s[nor.i] && (s[nor.i] == c))
	{
		s2[nor.j][nor.k] = s[nor.i];
		do_i_j(&nor.i, &nor.k);
	}
	while (s[nor.i] && (!(s[nor.i] == c) || nor.j == nor.wc - 1))
	{
		s2[nor.j][nor.k] = s[nor.i];
		do_i_j(&nor.i, &nor.k);
	}
	if (nor.k)
	{
		s2[nor.j][nor.k] = 0;
		nor.j++;
	}
	do_someth2_remaining(s, s2, c, &nor);
}

char	**ft_split_for_expand(char const *s, char c)
{
	char	**s2;
	int		wc;

	if (!s)
		return (NULL);
	wc = ft_word_count(s, c);
	s2 = malloc(sizeof(char *) * (wc + 1));
	if (!s2)
		return (NULL);
	s2[wc] = NULL;
	if (do_someth1(s, s2, c, wc) == 0)
		return (NULL);
	do_someth2(s, s2, c, wc);
	return (s2);
}

#include "../includes/minishell.h"

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
		return 1;
	return (wc);
}

static int	do_someth1(const char *s, char **s2, char c,int wc)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 0;
	k   = 0;
	while (s[i] && (s[i] == c))
	{
		k++;
		i++;
	}
	while (s[i] && (!(s[i] == c) || j == wc - 1))
	{
		i++;
		k++;
	}
	
	if (k)
	{
		s2[j] = malloc(sizeof(char) * (k + 1));
		if (!s2[j])
		{
			ft_free(s2, j - 1);
			return (0);
		}
		j++;
	}
	while (s[i])
	{
		k = 0;
		while (s[i] == c)
			i++;
		while (s[i] && (!(s[i++] == c) || j == wc - 1))
		{
			k++;
		}
		if (k)
		{
			s2[j] = malloc(sizeof(char) * (k + 1));
			if (!s2[j])
			{
				ft_free(s2, j - 1);
				return (0);
			}
			j++;
		}
	}
	return (1);
}

static void	do_someth2(const char *s, char **s2, char c,int wc)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 0;
	k = 0;
	while (s[i] && (s[i] == c))
	{
		s2[j][k] = s[i];
		i++;
		k++;
	}
	while (s[i] && (!(s[i] == c) || j == wc - 1))
	{	
		s2[j][k] = s[i];
		i++;
		k++;
	}
	if (k)
	{
		s2[j][k] = 0;
		j++;
	}
	while (s[i])
	{
		k = 0;
		while (s[i] == c)
			i++;
		while (s[i] && (!(s[i] == c) || j == wc - 1))
		{
			s2[j][k] = s[i];
			i++;
			k++;
		}
		if (k)
		{
			s2[j][k] = 0;
			j++;
		}
	}
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
	if (do_someth1(s, s2, c,wc) == 0)
		return (NULL);
	do_someth2(s, s2, c,wc);
	return (s2);
}



// int main()
// {
// 	char **s = ft_split_for_expand(" ",' ');
// 		printf("p = %p\n",*s);
// 	while (*s)
// 	{
// 		printf("%s-\n",*s);
// 		s++;
// 		printf("p = %p\n",*s);
// 	}
// }
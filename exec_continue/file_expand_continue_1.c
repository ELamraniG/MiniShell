#include "../includes/minishell.h"

void	free_relc(t_file_relc *relc)
{
	free(relc->tmpint_free);
	free(relc->tmp_free);
}

void	ft_realloc_file(t_file_expd *fxpd, char *s)
{
	t_file_relc	relc;

	relc.i = 0;
	relc.new_is_space = malloc(sizeof(int) * (fxpd->size + 1));
	relc.new_q_types = malloc(sizeof(int) * (fxpd->size + 1));
	relc.new_file_name = malloc(sizeof(char *) * (fxpd->size + 2));
	relc.new_file_name[fxpd->size] = 0;
	while (relc.i < fxpd->size)
	{
		relc.new_q_types[relc.i] = fxpd->q_types[relc.i];
		relc.new_is_space[relc.i] = fxpd->is_space[relc.i];
		relc.new_file_name[relc.i] = fxpd->file_name[relc.i];
		relc.i++;
	}
	relc.new_file_name[relc.i] = (s);
	relc.new_file_name[fxpd->size + 1] = 0;
	(fxpd->size)++;
	relc.tmp_free = fxpd->file_name;
	fxpd->file_name = relc.new_file_name;
	relc.tmpint_free = fxpd->is_space;
	fxpd->is_space = relc.new_is_space;
	free(relc.tmpint_free); 
	relc.tmpint_free = fxpd->q_types;
	fxpd->q_types = relc.new_q_types;
	return free_relc(&relc);
}

void	trim_the_file_names(t_file_expd *fxpd)
{
	char	*tmp_free;

	tmp_free = fxpd->file_name[fxpd->size - 1];
	fxpd->file_name[fxpd->size - 1] = ft_strtrim(fxpd->file_name[fxpd->size - 1], " ");
	free(tmp_free);
	return ;
}

char	*get_keyy2(char *str, t_expd2 *expd2)
{
	int		len;

	(expd2->i)++;
	expd2->tmp = NULL;
	len = 0;
	if (str[expd2->i] == '?')
	{
		(expd2->i)++;
		return (ft_strdup("?"));
	}
	if (!ft_isalpha(str[expd2->i]))
		return (ft_strdup("$"));
	while (str[expd2->i])
	{
		if (str[expd2->i] == '$' || (!ft_isalnum(str[expd2->i]) && str[expd2->i] != '_'))
			return (ft_substr(str, expd2->prev_pos + 1, expd2->i - expd2->prev_pos - 1));
		len++;
		(expd2->i)++;
	}
	if (len == 0)
		return (ft_strdup("$"));
	return (ft_strdup(str + expd2->prev_pos + 1));
}

int	has_space_at_the_end2(char *s)
{
	int	strl;

	if (!s)
		return (0);
	strl = ft_strlen(s);
	if (strl == 0)
		return (0);
	if (s[strl - 1] == ' ')
		return (1);
	return (0);
}

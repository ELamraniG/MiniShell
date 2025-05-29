#include "../includes/minishell.h"

int	has_space_beginning(char *s)
{
	int	strl;

	if (!s)
		return (0);
	strl = ft_strlen(s);
	if (strl == 0)
		return (0);
	if (s[0] == ' ')
		return (1);
	return (0);
}

void	file_expandnorm1(t_file_expd *fxpd, t_redirect *redir)
{
	ft_realloc_file(fxpd, ft_strdup(redir->file_name[fxpd->k]));
	fxpd->is_space[fxpd->size - 1] = redir->is_space[fxpd->k];
	fxpd->q_types[fxpd->size - 1] = redir->q_types[fxpd->k];
}

void	file_expand_norm3(t_file_expd *fxpd, t_expd2 *expd2, t_redirect *redir)
{
	expd2->tmp = ft_substr(redir->file_name[fxpd->k], expd2->prev_pos, expd2->i - expd2->prev_pos);
	if (expd2->tmp[0] != 0)
	{
		ft_realloc_file(fxpd, expd2->tmp);
		fxpd->is_space[fxpd->size - 1] = 0;
		fxpd->q_types[fxpd->size - 1] = redir->q_types[fxpd->k];
	}
	else
		free(expd2->tmp);
}

void	file_expand_norm7(t_file_expd *fxpd, t_expd2 *expd2)
{
	expd2->tmp3 = ft_strdup("");
	ft_realloc_file(fxpd, expd2->tmp3);
	free(expd2->tmp2);
	fxpd->is_space[fxpd->size - 1] = 0;
	fxpd->q_types[fxpd->size - 1] = DQ;
	expd2->prev_pos = expd2->i;
}

int file_expand_norm4(t_file_expd *fxpd, t_expd2 *expd2, t_redirect *redir, t_env_list *env)
{
	expd2->tmp2 = get_keyy2(redir->file_name[fxpd->k], expd2);
	if (redir->file_name[fxpd->k][expd2->i])
		expd2->flag = 0;
	if (!ft_strcmp(expd2->tmp2, "?"))
	{
		free(expd2->tmp2);
		expd2->tmp3 = ft_itoa(fxpd->status);
		expd2->flag = 0;
		return 1;
	}
	else if (!ft_strcmp(expd2->tmp2, "$"))
	{
		free(expd2->tmp2);
		expd2->tmp3 = ft_strdup("$");
		expd2->flag = 0;
		return 1;
	}
	else
	{
		expd2->t = get_env_value(env, expd2->tmp2);
		if (!expd2->t)
		{
			file_expand_norm7(fxpd, expd2);
			return 0;
		}
		else
			expd2->tmp3 = ft_strdup(expd2->t->value);
	}
	free(expd2->tmp2);
	return 0;
}

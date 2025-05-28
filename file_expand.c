#include "includes/minishell.h"


static void free_relc(t_file_relc *relc)
{
	free(relc->tmpint_free);
	free(relc->tmp_free);
}

static void	ft_realloc_file(t_file_expd *fxpd, char *s)
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

static void	trim_the_file_names(t_file_expd *fxpd)
{
	char	*tmp_free;

	tmp_free = fxpd->file_name[fxpd->size - 1];
	fxpd->file_name[fxpd->size - 1] = ft_strtrim(fxpd->file_name[fxpd->size - 1], " ");
	free(tmp_free);
	return ;
}

static char	*get_keyy(char *str, t_env_list *env, t_expd2 *expd2,
		int status)
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

static int	has_space_at_the_end(char *s)
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

static int	has_space_at_the_beginning(char *s)
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

static void	file_expandnorm1(t_file_expd *fxpd, t_redirect *redir)
{
	ft_realloc_file(fxpd, ft_strdup(redir->file_name[fxpd->k]));
	fxpd->is_space[fxpd->size - 1] = redir->is_space[fxpd->k];
	fxpd->q_types[fxpd->size - 1] = redir->q_types[fxpd->k];
}

static void	file_expand_norm3(t_file_expd *fxpd, t_expd2 *expd2, t_redirect *redir)
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

static void	file_expand_norm7(t_file_expd *fxpd, t_expd2 *expd2)
{
	expd2->tmp3 = ft_strdup("");
	ft_realloc_file(fxpd, expd2->tmp3);
	free(expd2->tmp2);
	fxpd->is_space[fxpd->size - 1] = 0;
	fxpd->q_types[fxpd->size - 1] = DQ;
	expd2->prev_pos = expd2->i;
}

static void	file_expand_norm4(t_file_expd *fxpd, t_expd2 *expd2, t_redirect *redir, t_env_list *env, int status)
{
	expd2->tmp2 = get_keyy(redir->file_name[fxpd->k], env, expd2, status);
	if (redir->file_name[fxpd->k][expd2->i])
		expd2->flag = 0;
	if (!ft_strcmp(expd2->tmp2, "?"))
	{
		expd2->tmp3 = ft_itoa(status);
		expd2->flag = 0;
	}
	else if (!ft_strcmp(expd2->tmp2, "$"))
	{
		expd2->tmp3 = ft_strdup("$");
		expd2->flag = 0;
	}
	else
	{
		expd2->t = get_env_value(env, expd2->tmp2);
		if (!expd2->t)
		{
			file_expand_norm7(fxpd, expd2);
			return;
		}
		else
			expd2->tmp3 = ft_strdup(expd2->t->value);
	}
	free(expd2->tmp2);
}

static void	file_expand_norm5(t_file_expd *fxpd, t_expd2 *expd2)
{
	expd2->dble = ft_split_for_expand(expd2->tmp3, ' ');
	free(expd2->tmp3);
	while (expd2->dble[expd2->j])
	{
		ft_realloc_file(fxpd, expd2->dble[expd2->j]);
		fxpd->is_space[fxpd->size - 1] = 1;
		fxpd->q_types[fxpd->size - 1] = NQ;
		if (expd2->j == 0)
		{
			if (has_space_at_the_beginning(fxpd->file_name[fxpd->size - 1])
				&& fxpd->size > 1)
				fxpd->is_space[fxpd->size - 2] = 1;
		}
		expd2->j++;
		if (expd2->dble[expd2->j] == NULL)
		{
			if (has_space_at_the_end(fxpd->file_name[fxpd->size - 1]))
				fxpd->is_space[fxpd->size - 1] = 1;
			else
				fxpd->is_space[fxpd->size - 1] = 0;
		}
		trim_the_file_names(fxpd);
	}
	free(expd2->dble);
}

static void	file_expand_norm2(t_file_expd *fxpd, t_expd2 *expd2, t_redirect *redir, t_env_list *env, int status)
{
	expd2->flag = 1;
	file_expand_norm3(fxpd, expd2, redir);
	expd2->prev_pos = expd2->i;
	file_expand_norm4(fxpd, expd2, redir, env, status);
	if (expd2->t && expd2->t->value)
	{
		expd2->j = 0;
		file_expand_norm5(fxpd, expd2);
	}
	expd2->prev_pos = expd2->i;
}

static void	file_expand_norm6(t_file_expd *fxpd, t_expd2 *expd2, t_redirect *redir)
{
	if (expd2->flag == 0)
	{
		expd2->tmp = ft_substr(redir->file_name[fxpd->k], expd2->prev_pos, ft_strlen(redir->file_name[fxpd->k]) - expd2->prev_pos);
		ft_realloc_file(fxpd, expd2->tmp);
		fxpd->is_space[fxpd->size - 1] = redir->is_space[fxpd->k];
		fxpd->q_types[fxpd->size - 1] = redir->q_types[fxpd->k];
	}
	fxpd->is_space[fxpd->size - 1] = redir->is_space[fxpd->k];
}

static void	expanded_for_single_file(t_file_expd *fxpd, t_env_list *env,
		int status, t_redirect *redir)
{
	t_expd2 expd2;

	expd2.i = 0;
	if (redir->q_types[fxpd->k] == SQ)
	{
		file_expandnorm1(fxpd, redir);
		return ;
	}
	expd2.flag = 0;
	expd2.prev_pos = 0;
	while (redir->file_name[fxpd->k][expd2.i])
	{
		if (redir->file_name[fxpd->k][expd2.i] == '\'')
			expd2.i++;
		else if (redir->file_name[fxpd->k][expd2.i] == '$')
			file_expand_norm2(fxpd, &expd2, redir, env, status);
		else
			expd2.i++;
	}
	file_expand_norm6(fxpd, &expd2, redir);
}

static char *join_all_names(char **file_name, int count)
{
	char *tmp = ft_strdup("");
	char *tmp_free;
	int i = 0;
	while (i < count)
	{
		tmp_free = tmp;
		tmp = ft_strjoin(tmp,file_name[i]);
		free(tmp_free);
		i++;
	}
	return tmp;
}

static void	check_ambiguous_redirect(t_redirect *redir, t_file_expd *fxpd, int *abg)
{
	int i;

	i = 0;
	*abg = -1;
	while (i < redir->file_str_count)
	{
		if (redir->q_types[i] != NQ)
			*abg = 0;
		i++;
	}
	i = 0;
	while (i < fxpd->size)
	{
		if (fxpd->file_name[i][0] != 0)
			*abg = 0;
		i++;
	}
}

static void	cleanup_and_assign_file_data(t_redirect *redir, t_file_expd *fxpd)
{
	while (fxpd->k < redir->file_str_count)
		free(redir->file_name[fxpd->k++]);
	free(redir->file_name);
	redir->file_name = fxpd->file_name;
	redir->file_str_count = fxpd->size;
	free(redir->is_space);
	redir->is_space = fxpd->is_space;
	free(redir->q_types);
	redir->q_types = fxpd->q_types;
}

static int	I_HATE_EXPANDING_FILE(t_redirect *redir, t_env_list *env,
		int status)
{
	t_file_expd fxpd;
	int		abg;

	fxpd.k = 0;
	fxpd.file_name = NULL;
	fxpd.size = 0;
	fxpd.is_space = NULL;
	fxpd.q_types = NULL;
	while (fxpd.k < redir->file_str_count)
	{
		expanded_for_single_file(&fxpd, env, status, redir);
		fxpd.k++;
	}
	fxpd.k = 0;
	check_ambiguous_redirect(redir, &fxpd, &abg);
	cleanup_and_assign_file_data(redir, &fxpd);
	return abg;
}

int	expand_file_name(t_ast_tree *node, t_env_list *env, int status)
{
	t_redirect *redir = node->redirect;
	int abg = 0;
	while (redir)
	{
		if (redir->type != HEREDOC)
		{
			abg = I_HATE_EXPANDING_FILE(redir, env, status);
			if (abg != 0)
				return -1;
		}
		redir = redir->next;
	}
	return (0);
}

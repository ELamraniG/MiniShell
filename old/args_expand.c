#include "includes/minishell.h"

static void	free_relc(t_relc *relc)
{
	free(relc->tmpint_free);
	free(relc->tmp_free);
}

void	ft_realloc(t_expd_norm *expd, char *s)
{
	t_relc	relc;

	relc.i = 0;
	relc.new_is_space = malloc(sizeof(int) * (expd->size + 1));
	relc.new_q_type = malloc(sizeof(int) * (expd->size + 1));
	relc.new_args = malloc(sizeof(char *) * (expd->size + 2));
	relc.new_args[expd->size] = 0;
	while (relc.i < expd->size)
	{
		relc.new_q_type[relc.i] = expd->q_type[relc.i];
		relc.new_is_space[relc.i] = expd->is_space[relc.i];
		relc.new_args[relc.i] = expd->args[relc.i];
		relc.i++;
	}
	relc.new_args[relc.i] = (s);
	relc.new_args[expd->size + 1] = 0;
	(expd->size)++;
	relc.tmp_free = expd->args;
	expd->args = relc.new_args;
	relc.tmpint_free = expd->is_space;
	expd->is_space = relc.new_is_space;
	free(relc.tmpint_free); 
	relc.tmpint_free = expd->q_type;
	expd->q_type = relc.new_q_type;
	return free_relc(&relc);
}

static void	trim_the_args(t_expd_norm *expd)
{
	char	*tmp_free;

	tmp_free = expd->args[expd->size - 1];
	expd->args[expd->size - 1] = ft_strtrim(expd->args[expd->size - 1], " ");
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


static void	expandnorm1(t_expd_norm *expd, t_ast_tree *astree)
{
	ft_realloc(expd, ft_strdup(astree->args[expd->k]));
	expd->is_space[expd->size - 1] = astree->is_space[expd->k];
	expd->q_type[expd->size - 1] = astree->q_type[expd->k];
}

static void	expand_norm3(t_expd_norm *expd, t_expd2 *expd2, t_ast_tree *astree)
{
	expd2->tmp = ft_substr(astree->args[expd->k], expd2->prev_pos, expd2->i - expd2->prev_pos);
	if (expd2->tmp[0] != 0)
	{
		ft_realloc(expd, expd2->tmp);
		expd->is_space[expd->size - 1] = 0;
		expd->q_type[expd->size - 1] = astree->q_type[expd->k];
	}
	else
		free(expd2->tmp);
}

static void	expand_norm7(t_expd_norm *expd, t_expd2 *expd2)
{
	expd2->tmp3 = ft_strdup("");
	ft_realloc(expd, expd2->tmp3);
	free(expd2->tmp2);
	expd->is_space[expd->size - 1] = 0;
	expd->q_type[expd->size - 1] = DQ;
	expd2->prev_pos = expd2->i;
}

static int	expand_norm4(t_expd_norm *expd, t_expd2 *expd2, t_ast_tree *astree, t_env_list *env, int status)
{
	expd2->tmp2 = get_keyy(astree->args[expd->k], env, expd2, status);
	if (astree->args[expd->k][expd2->i])
		expd2->flag = 0;
	if (!ft_strcmp(expd2->tmp2, "?"))
	{
		expd2->tmp3 = ft_itoa(status);
		expd2->flag = 0;
		return (1);
	}
	else if (!ft_strcmp(expd2->tmp2, "$"))
	{
		expd2->tmp3 = ft_strdup("$");
		expd2->flag = 0;
		return (1);
	}
	else
	{
		expd2->t = get_env_value(env, expd2->tmp2);
		if (!expd2->t)
		{
			expand_norm7(expd, expd2);
			return 0;
		}
		else
			expd2->tmp3 = ft_strdup(expd2->t->value);
	}
	free(expd2->tmp2);
	return 0;
}

static void	expand_norm5(t_expd_norm *expd, t_expd2 *expd2)
{
	expd2->j = 0;
	expd2->dble = ft_split_for_expand(expd2->tmp3, ' ');
	free(expd2->tmp3);
	while (expd2->dble[expd2->j])
	{
		ft_realloc(expd, expd2->dble[expd2->j]);
		expd->is_space[expd->size - 1] = 1;
		expd->q_type[expd->size - 1] = NQ;
		if (expd2->j == 0)
		{
			if (has_space_at_the_beginning(expd->args[expd->size - 1])
				&& expd->size > 1)
				expd->is_space[expd->size - 2] = 1;
		}
		expd2->j++;
		if (expd2->dble[expd2->j] == NULL)
		{
			if (has_space_at_the_end(expd->args[expd->size - 1]))
				expd->is_space[expd->size - 1] = 1;
			else
				expd->is_space[expd->size - 1] = 0;
		}
		trim_the_args(expd);
	}
	free(expd2->dble);
}

static void	expand_norm2(t_expd_norm *expd, t_expd2 *expd2, t_ast_tree *astree, t_env_list *env, int status)
{
	int kk;
	expd2->flag = 1;
	expand_norm3(expd, expd2, astree);
	expd2->prev_pos = expd2->i;
	kk = expand_norm4(expd, expd2, astree, env, status);
	if (kk == 0)
		expand_norm5(expd, expd2);
	else if (expd2->t && expd2->t->value)
	{
		ft_realloc_file(expd, expd2->tmp3);
		expd->is_space[expd->size - 1] = 0;
		expd->q_type[expd->size - 1] = astree->is_space[expd->k];
	}
	expd2->prev_pos = expd2->i;
}

static void	expand_norm6(t_expd_norm *expd, t_expd2 *expd2, t_ast_tree *astree)
{
	if (expd2->flag == 0)
	{
		expd2->tmp = ft_substr(astree->args[expd->k], expd2->prev_pos, ft_strlen(astree->args[expd->k]) - expd2->prev_pos);
		ft_realloc(expd, expd2->tmp);
		expd->is_space[expd->size - 1] = astree->is_space[expd->k];
		expd->q_type[expd->size - 1] = astree->q_type[expd->k];
	}
	expd->is_space[expd->size - 1] = astree->is_space[expd->k];
}

static void	expanded_for_single_word(t_expd_norm *expd, t_env_list *env,
		int status, t_ast_tree *astree)
{
	t_expd2 expd2;

	expd2.i = 0;
	expd2.t = NULL;
	if (astree->q_type[expd->k] == SQ)
	{
		expandnorm1(expd, astree);
		return ;
	}
	expd2.flag = 0;
	expd2.prev_pos = 0;
	while (astree->args[expd->k][expd2.i])
	{
		if (astree->args[expd->k][expd2.i] == '\'')
			expd2.i++;
		else if (astree->args[expd->k][expd2.i] == '$')
			expand_norm2(expd, &expd2, astree, env, status);
		else
			expd2.i++;
	}
	expand_norm6(expd, &expd2, astree);
}



void	I_HATE_EXPANDING(t_ast_tree *astree, t_env_list *env, int status)
{
	t_expd_norm expd;

	expd.k = 0;
	expd.args = NULL;
	expd.size = 0;
	expd.is_space = NULL;
	expd.k = 0;
	expd.q_type = NULL;
	while (astree->args[expd.k])
	{
		expanded_for_single_word(&expd,env, status,astree);
		expd.k++;
	}
	expd.k = 0;
	while (astree->args[expd.k])
		free(astree->args[expd.k++]);
	free(astree->args[expd.k]);
	free(astree->args);
	astree->args = expd.args;
	astree->arg_counter = expd.size;
	free(astree->is_space);
	astree->is_space = expd.is_space;
	free(astree->q_type);
	astree->q_type = expd.q_type;
}

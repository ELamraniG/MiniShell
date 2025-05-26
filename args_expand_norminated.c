#include "includes/minishell.h"

void	ft_realloc(t_expd_norm *expd, char *s)
{
	int		i;
	int		*new_is_space;
	char	**new_args;
	int		*tmpint_free;
	char	**tmp_free;
	int 	*new_q_type;

	i = 0;
	new_is_space = malloc(sizeof(int) * (expd->size + 1));
	new_q_type = malloc(sizeof(int) * (expd->size + 1));
	new_args = malloc(sizeof(char *) * (expd->size + 2));
	new_args[expd->size] = 0;
	while (i < expd->size)
	{
		new_q_type[i] = expd->q_type[i];
		new_is_space[i] = expd->is_space[i];
		new_args[i] = expd->args[i];
		i++;
	}
	new_args[i] = (s);
	new_args[expd->size + 1] = 0;
	(expd->size)++;
	tmp_free = expd->args;
	expd->args = new_args;
	tmpint_free = expd->is_space;
	expd->is_space = new_is_space;
	free(tmpint_free); 
	tmpint_free = expd->q_type;
	expd->q_type = new_q_type;
	free(tmpint_free);
	free(tmp_free);
	return ;
}

static void	trim_the_args(t_expd_norm *expd)
{
	char	*tmp_free;

	tmp_free = expd->args[expd->size - 1];
	expd->args[expd->size - 1] = ft_strtrim(expd->args[expd->size - 1], " ");
	free(tmp_free);
	return ;
}

static char	*get_keyy(char *str, t_env_list *env, int prev_pos, int *i,
		int status)
{
	char	*tmp;
	int		len;

	(*i)++;
	tmp = NULL;
	// i skip the $
	len = 0;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	if (!ft_isalpha(str[*i]))
		return (ft_strdup("$"));
	while (str[*i])
	{
		if (str[*i] == '$' || !ft_isalnum(str[*i]))
			return (ft_substr(str, prev_pos + 1, *i - prev_pos - 1));
		len++;
		(*i)++;
	}
	if (len == 0)
		return (ft_strdup("$"));
	return (ft_strdup(str + prev_pos + 1));
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

static void	expanded_for_single_word(t_expd_norm *expd, t_env_list *env,
		int status, t_ast_tree *astree)
{
	int		i;
	int		j;
	int		flag;
	int		prev_pos;
	char	**dble;
	char	*tmp3;
	char	*tmp;
	char	*tmp2;
				t_env_list *t;

	i = 0;
	j = 0;
	if (astree->q_type[expd->k] == SQ)
	{
		ft_realloc(expd, ft_strdup(astree->args[expd->k]));
		expd->is_space[expd->size - 1] = astree->is_space[expd->k];
		expd->q_type[expd->size - 1] = astree->q_type[expd->k];
		return ;
	}
	flag = 0;
	prev_pos = 0;
	while (astree->args[expd->k][i])
	{
		j = 0;
		if (astree->args[expd->k][i] == '\'')
			i++;
		else if (astree->args[expd->k][i] == '$')
		{
			flag = 1;
			tmp = ft_substr(astree->args[expd->k], prev_pos, i - prev_pos);
			prev_pos = i;
			if (tmp[0] != 0)
			{
				ft_realloc(expd, tmp);
				expd->is_space[expd->size - 1] = 0;
				expd->q_type[expd->size - 1] = astree->q_type[expd->k];
			}
			else
				free(tmp);
			tmp2 = get_keyy(astree->args[expd->k], env, prev_pos, &i, status);
			if (astree->args[expd->k][i])
				flag = 0;
			if (!ft_strcmp(tmp2, "?"))
			{
				tmp3 = ft_itoa(status);
				flag = 0;
			}
			else
			{
				t = get_env_value(env, tmp2);
				if (!t)
				{
					tmp3 = ft_strdup("");
					ft_realloc(expd, tmp3);
					free(tmp2);
					expd->is_space[expd->size - 1] = 0;
					expd->q_type[expd->size - 1] = DQ;
					prev_pos = i;
					continue ;
				}
				else
					tmp3 = ft_strdup(t->value);
			}
			free(tmp2);
			dble = ft_split_for_expand(tmp3, ' ');
			free(tmp3);
			while (dble[j])
			{
				ft_realloc(expd, dble[j]);
				expd->is_space[expd->size - 1] = 1;
				expd->q_type[expd->size - 1] = NQ;
				if (j == 0)
				{
					if (has_space_at_the_beginning(expd->args[expd->size - 1])
						&& expd->size > 1)
						expd->is_space[expd->size - 2] = 1;
				}
				j++;
				if (dble[j] == NULL)
				{
					if (has_space_at_the_end(expd->args[expd->size - 1]))
						expd->is_space[expd->size - 1] = 1;
					else
						expd->is_space[expd->size - 1] = 0;
				}
				trim_the_args(expd);
			}
			free(dble);
			prev_pos = i;
		}
		else
			i++;
	}
	if (flag == 0)
	{
		tmp = ft_substr(astree->args[expd->k], prev_pos, ft_strlen(astree->args[expd->k]) - prev_pos);
		ft_realloc(expd, tmp);
		expd->is_space[expd->size - 1] = astree->is_space[expd->k];
		expd->q_type[expd->size - 1] = astree->q_type[expd->k];
	}
	expd->is_space[expd->size - 1] = astree->is_space[expd->k];
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

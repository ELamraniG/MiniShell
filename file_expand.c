#include "includes/minishell.h"

static void	ft_realloc(char ***args, char *s, int *size, int **is_space)
{
	int		i;
	int		*new_is_space;
	char	**new_args;
	int		*tmpint_free;
	char	**tmp_free;

	i = 0;
	new_is_space = malloc(sizeof(int) * (*size + 1));
	new_args = malloc(sizeof(char *) * (*size + 2));
	new_args[*size] = 0;
	while (i < *size)
	{
		new_is_space[i] = is_space[0][i];
		new_args[i] = args[0][i];
		i++;
	}
	new_args[i] = ft_strdup(s);
	new_args[*size + 1] = 0;
	(*size)++;
	tmp_free = *args;
	*args = new_args;
	tmpint_free = is_space[0];
	*is_space = new_is_space;
	free(tmpint_free);
	free(tmp_free);
	return ;
}

static void	trim_the_args(char ***args, int size)
{
	char	*tmp_free;

	tmp_free = args[0][size - 1];
	args[0][size - 1] = ft_strtrim(args[0][size - 1], " ");
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

static void	expanded_for_single_word(char ***args, char *str, t_env_list *env,
		int status, int old_is_space, int k, int *size, int q_type,
		int **is_space)
{
	int			i;
	int			j;
	int			flag;
	int			prev_pos;
	char		**dble;
	char		*tmp3;
	char		*tmp;
	char		*tmp2;
	t_env_list	*t;

	i = 0;
	j = 0;
	if (q_type == SQ)
	{
		ft_realloc(args, ft_strdup(str), size, is_space);
		is_space[0][*size - 1] = old_is_space;
		return ;
	}
	flag = 0;
	prev_pos = 0;
	while (str[i])
	{
		j = 0;
		if (str[i] == '\'')
			i++;
		else if (str[i] == '$')
		{
			flag = 1;
			tmp = ft_substr(str, prev_pos, i - prev_pos);
			prev_pos = i;
			if (tmp[0] != 0)
			{
				ft_realloc(args, tmp, size, is_space);
				is_space[0][*size - 1] = 0;
			}
			else
				free(tmp);
			tmp2 = get_keyy(str, env, prev_pos, &i, status);
			if (str[i])
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
					ft_realloc(args, tmp3, size, is_space);
					free(tmp2);
					is_space[0][*size - 1] = 0;
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
				ft_realloc(args, dble[j], size, is_space);
				is_space[0][*size - 1] = 1;
				if (j == 0)
				{
					if (has_space_at_the_beginning(args[0][*size - 1])
						&& *size > 1)
						is_space[0][*size - 2] = 1;
				}
				j++;
				if (dble[j] == NULL)
				{
					if (has_space_at_the_end(args[0][*size - 1]))
						is_space[0][*size - 1] = 1;
					else
						is_space[0][*size - 1] = 0;
				}
				trim_the_args(args, *size);
			}
			free(dble);
			prev_pos = i;
		}
		else
			i++;
	}
	if (flag == 0)
	{
		tmp = ft_substr(str, prev_pos, ft_strlen(str) - prev_pos);
		ft_realloc(args, tmp, size, is_space);
		is_space[0][*size - 1] = old_is_space;
	}
	is_space[0][*size - 1] = old_is_space;
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

static int	I_HATE_EXPANDING_FILE(t_redirect *redir, t_env_list *env,
		int status)
{
	int		k;
	char	**file_name;
	int		size;
	int		*is_space;
	int		i;
	int		abg;

	k = 0;
	file_name = NULL;
	size = 0;
	is_space = NULL;
	k = 0;
	i = 0;
	abg = -1;
	while (k < redir->file_str_count)
	{
		expanded_for_single_word(&file_name, redir->file_name[k], env, status,
			redir->is_space[k], k, &size, redir->q_types[k], &is_space);
		k++;
	}
	while (i < redir->file_str_count)
	{
		if (redir->q_types[i] != NQ)
			abg = 0;
		i++;
	}
	i = 0;
	while (i < size)
	{
		if (file_name[i][0] != 0)
			abg = 0;
		i++;
	}
	if (abg == -1 || size > 1)
	{
		abg = -1;
		char *tmp = join_all_names(redir->file_name,redir->file_str_count);
		ft_putstr_fd(2, tmp);
		free(tmp);
		ft_putstr_fd(2, ": ambiguous redirect\n");
	}
	k = 0;
	while (k < redir->file_str_count)
		free(redir->file_name[k++]);
	free(redir->file_name);
	redir->file_name = file_name;
	redir->file_str_count = size;
	free(redir->is_space);
	redir->is_space = is_space;
	free(redir->q_types);
	redir->q_types = malloc(sizeof(int) * size);
	return abg;
}
int	expand_file_name(t_ast_tree *node, t_env_list *env, int status)
{
	t_redirect *redir = node->redirect;
	int abg = 0;
	while (redir)
	{
		abg = I_HATE_EXPANDING_FILE(redir, env, status);
		if (abg != 0)
			return -1;
		redir = redir->next;
	}
	return (0);
}
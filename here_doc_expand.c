#include "includes/minishell.h"


static char *get_keyy(char *str, int prev_pos, int *i)
{
	(*i)++;
	int len = 0;
	if (str[*i] == '?')
	{
		(*i)++;
		return ft_strdup("?");
	}
	if (!ft_isalpha(str[*i]))
		return ft_strdup("$");
	while (str[*i])
	{
		if (str[*i] == '$' || (!ft_isalnum(str[*i]) && str[*i] != '_'))
			return ft_substr(str, prev_pos + 1, *i - prev_pos - 1);
		len++;
		(*i)++;
	}
	if (len == 0)
		return ft_strdup("$");
	return (ft_strdup(str + prev_pos + 1));
}

static void	expand_heredoc_variable(t_heredoc_norm *heredoc, char *s, t_env_list *env, int status)
{
	heredoc->flag = 1;
	heredoc->tmp = ft_substr(s, heredoc->prev_pos, heredoc->i - heredoc->prev_pos);
	heredoc->tmp_free = heredoc->str;
	heredoc->str = ft_strjoin(heredoc->str, heredoc->tmp);
	free(heredoc->tmp);
	free(heredoc->tmp_free);
	
	heredoc->prev_pos = heredoc->i;
	heredoc->key = get_keyy(s, heredoc->prev_pos, &heredoc->i);
	
	if (!ft_strcmp(heredoc->key, "?"))
		heredoc->tmp2 = ft_itoa(status);
	else if (!ft_strcmp(heredoc->key, "$"))
		heredoc->tmp2 = ft_strdup("$");
	else
	{
		t_env_list *env_node = get_env_value(env, heredoc->key);
		if (!env_node)
			heredoc->tmp2 = ft_strdup("");
		else
			heredoc->tmp2 = ft_strdup(env_node->value);
	}
	
	heredoc->tmp_free = heredoc->str;
	heredoc->str = ft_strjoin(heredoc->str, heredoc->tmp2);
	free(heredoc->tmp_free);
	free(heredoc->key);
	free(heredoc->tmp2);
	heredoc->prev_pos = heredoc->i;
}

char *expand_heredoc_line(char *s, t_env_list *env, int status)
{
	t_heredoc_norm heredoc;

	heredoc.i = 0;
	heredoc.prev_pos = 0;
	heredoc.flag = 0;
	heredoc.str = ft_strdup("");
	
	while (s[heredoc.i])
	{
		if (s[heredoc.i] == '$')
		{
			expand_heredoc_variable(&heredoc, s, env, status);
		}
		else
			heredoc.i++;
	}
	
	if (heredoc.flag == 0)
	{
		heredoc.tmp = ft_substr(s, heredoc.prev_pos, ft_strlen(s) - heredoc.prev_pos);
		heredoc.tmp_free = heredoc.str;
		heredoc.str = ft_strjoin(heredoc.str, heredoc.tmp);
		free(heredoc.tmp);
		free(heredoc.tmp_free);
	}
	return heredoc.str;
}

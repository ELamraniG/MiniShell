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
		if (str[*i] == '$' || !ft_isalnum(str[*i]))
			return ft_substr(str, prev_pos + 1, *i - prev_pos - 1);
		len++;
		(*i)++;
	}
	if (len == 0)
		return ft_strdup("$");
	return (ft_strdup(str + prev_pos + 1));
}

char *expand_heredoc_line(char *s, t_env_list *env, int status)
{
	int i = 0;
	int prev_pos = 0;
	int flag = 0;
	char *str = ft_strdup("");
	char *tmp;
	char *tmp_free;
	char *key;
	char *tmp2;
	
	while (s[i])
	{
		if (s[i] == '$')
		{
			flag = 1;
			tmp = ft_substr(s, prev_pos, i - prev_pos);
			tmp_free = str;
			str = ft_strjoin(str, tmp);
			free(tmp);
			free(tmp_free);
			
			prev_pos = i;
			key = get_keyy(s, prev_pos, &i);
			
			if (!ft_strcmp(key, "?"))
				tmp2 = ft_itoa(status);
			else
			{
				t_env_list *env_node = get_env_value(env, key);
				if (!env_node)
					tmp2 = ft_strdup("");
				else
					tmp2 = ft_strdup(env_node->value);
			}
			
			tmp_free = str;
			str = ft_strjoin(str, tmp2);
			free(tmp_free);
			free(key);
			free(tmp2);
			prev_pos = i;
		}
		else
			i++;
	}
	
	if (flag == 0 || prev_pos > 0)
	{
		tmp = ft_substr(s, prev_pos, ft_strlen(s) - prev_pos);
		tmp_free = str;
		str = ft_strjoin(str, tmp);
		free(tmp);
		free(tmp_free);
	}
	else
	{
		free(str);
		return ft_strdup(s);
	}
	return str;
}

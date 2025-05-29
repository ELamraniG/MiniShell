/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_expand_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 20:49:37 by moel-amr          #+#    #+#             */
/*   Updated: 2025/05/29 23:09:41 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*get_keyy(char *str, int prev_pos, int *i)
{
	int	len;

	(*i)++;
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
		if (str[*i] == '$' || (!ft_isalnum(str[*i]) && str[*i] != '_'))
			return (ft_substr(str, prev_pos + 1, *i - prev_pos - 1));
		len++;
		(*i)++;
	}
	if (len == 0)
		return (ft_strdup("$"));
	return (ft_strdup(str + prev_pos + 1));
}

static void	norm_free195(t_hrdc_norm *hrdc)
{
	free(hrdc->tmp_free);
	free(hrdc->key);
	free(hrdc->tmp2);
	hrdc->prev_pos = hrdc->i;
}

static void	expand_heredoc_variable(t_hrdc_norm *hrdc, char *s, t_env_list *env,
		int status)
{
	t_env_list	*env_node;

	hrdc->flag = 1;
	hrdc->tmp = ft_substr(s, hrdc->prev_pos, hrdc->i - hrdc->prev_pos);
	hrdc->tmp_free = hrdc->str;
	hrdc->str = ft_strjoin(hrdc->str, hrdc->tmp);
	free(hrdc->tmp);
	free(hrdc->tmp_free);
	hrdc->prev_pos = hrdc->i;
	hrdc->key = get_keyy(s, hrdc->prev_pos, &hrdc->i);
	if (!ft_strcmp(hrdc->key, "?"))
		hrdc->tmp2 = ft_itoa(status);
	else if (!ft_strcmp(hrdc->key, "$"))
		hrdc->tmp2 = ft_strdup("$");
	else
	{
		env_node = get_env_value(env, hrdc->key);
		if (!env_node)
			hrdc->tmp2 = ft_strdup("");
		else
			hrdc->tmp2 = ft_strdup(env_node->value);
	}
	hrdc->tmp_free = hrdc->str;
	hrdc->str = ft_strjoin(hrdc->str, hrdc->tmp2);
	norm_free195(hrdc);
}

char	*expand_heredoc_line(char *s, t_env_list *env, int status)
{
	t_hrdc_norm	hrdc;

	hrdc.i = 0;
	hrdc.prev_pos = 0;
	hrdc.flag = 0;
	hrdc.str = ft_strdup("");
	while (s[hrdc.i])
	{
		if (s[hrdc.i] == '$')
		{
			expand_heredoc_variable(&hrdc, s, env, status);
		}
		else
			hrdc.i++;
	}
	if (hrdc.flag == 0)
	{
		hrdc.tmp = ft_substr(s, hrdc.prev_pos, ft_strlen(s) - hrdc.prev_pos);
		hrdc.tmp_free = hrdc.str;
		hrdc.str = ft_strjoin(hrdc.str, hrdc.tmp);
		free(hrdc.tmp);
		free(hrdc.tmp_free);
	}
	return (hrdc.str);
}

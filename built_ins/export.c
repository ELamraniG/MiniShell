#include "../includes/minishell.h"

static int	validate_key(char *s)
{
	int	i;

	i = 0;
	if (!ft_isalpha(s[i]) && s[i] != '_')
		return (0);
	i++;
	while (s[i])
	{
		if (s[i] != '_' && !ft_isalnum(s[i]))
			return (0);
		i++;
	}
	return (1);
}

char	*get_value_env(t_env_list**d, char *key) // function i give it key, it returns the value, if no value found return NULL
{
	t_env_list	*env;

	if (!key)
		return (NULL);
	env = *d;
	while (env)
	{
		if (!ft_strcmp(env->key, key))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	print_export(t_env_list *env_list)
{
	while (env_list)
	{
		if (env_list->key)
		{
			printf("declare -x %s", env_list->key);
			if (env_list->flag)
				printf("=\"%s\"\n", env_list->value);
			else
				printf("\n");
		}
		env_list = env_list->next;
	}
}

int	find_key(t_env_list *d, char *key)
{
	while (d)
	{
		if (!ft_strcmp(d->key, key))
			return (1);
		d = d->next;
	}
	return (0);
}

int	exec_export(t_env_list **env, char **args)
{
	int			i;
	int			j;
	char		*key;
	char		*value;
	t_env_list	*exists;

	i = 0;
	if (!args[1]) 
	{
		print_export(*env);
		return (0);
	}
	while (args[++i])
	{
		j = -1;
		while (args[i][++j])
		{
			if (args[i][j] == '=')
			{
				// handling empty key case (export =value)
				if (j == 0)
				{
					printf("minishell: export: `%s': not a valid identifier\n", args[i]);
					break;
				}
				// handling += case
				if (j > 0 && args[i][j - 1] == '+')
				{
					key = ft_substr(args[i], 0, j-1);
					if (!validate_key(key))
					{
						printf("minishell: export: `%s': not a valid identifier\n", key);
						free(key);
						break;
					}
					value = ft_substr(args[i], j+1, ft_strlen(args[i])-j);
					exists = get_env_value(*env, key);
					if (exists)
					{
						char *new_val = ft_strjoin(exists->value, value);
						free(exists->value);
						exists->value = new_val;
						exists->flag = 1;
					}
					else
						insert_node_last(env, key, value, 1);
					free(key);
					free(value);
				}
				// normal = case
				else
				{
					key = ft_substr(args[i], 0, j);
					if (!validate_key(key))
					{
						printf("export: `%s': not a valid identifier\n", key);
						free(key);
						break;
					}
					value = ft_substr(args[i], j+1, ft_strlen(args[i])-j);
					insert_node_last(env, key, value, 1);
					free(key);
					free(value);
				}
				break;
			}
		}
		// No = found, just key
		if (!args[i][j] && j > 0)
		{
			key = ft_strdup(args[i]);
			if (!validate_key(key))
			{
				printf("export: `%s': not a valid identifier\n", key);
				free(key);
				continue;
			}
			exists = get_env_value(*env, key);
			if (!exists)
				insert_node_last(env, key, "", 0);
			free(key);
		}
	}
	return (0);
}

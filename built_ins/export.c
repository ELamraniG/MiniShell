#include "../includes/minishell.h"

typedef enum e_export_status
{
    OK,
    CONTINUE,
    BREAK
} t_export_status;


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

t_export_status	handle_normal(char *args, t_env_list **env, int j)
{
	char	*key;
	char	*value;
	key = ft_substr(args, 0, j);
	if (!validate_key(key))
	{
		printf("export: `%s': not a valid identifier\n", key);
		free(key);
		return (BREAK);
	}
	value = ft_substr(args, j + 1, ft_strlen(args) - j);
	insert_node_last(env, key, value, 1);
	free(key);
	free(value);
	return (OK);
}

t_export_status	handle_join(char *args, t_env_list **env, int j)
{
	char		*key;
	char		*value;
	char		*new_val;
	t_env_list	*exists;

	key = ft_substr(args, 0, j - 1);
	if (!validate_key(key))
	{
		printf("minishell: export: `%s': not a valid identifier\n", key);
		free(key);
		return (BREAK);
	}
	value = ft_substr(args, j + 1, ft_strlen(args) - j);
	exists = get_env_value(*env, key);
	if (exists)
	{
		new_val = ft_strjoin(exists->value, value);
		free (exists->value);
		exists->value = new_val;
		exists->flag = 1;
	}
	else
		insert_node_last(env, key, value, 1);
	return (free(key), free(value), OK);
}

t_export_status	handle_key_alone(char *args, t_env_list **env)
{
	char		*key;
	t_env_list	*exists;

	key = ft_strdup(args);
	if (!validate_key(key))
	{
		printf("export: `%s': not a valid identifier\n", key);
		free(key);
		return (CONTINUE) ;
	}
	exists = get_env_value(*env, key);
	if (!exists)
		insert_node_last(env, key, "", 0);
	free(key);
	return (OK);
}

void	print_msg(char *args)
{
	ft_putstr_fd(2, "minishell: export: ");
	ft_putstr_fd(2, args);
	ft_putstr_fd(2, "not a valid identifier\n");
}

t_export_status	handle_export(char **args, t_env_list **env, int i, int *j)
{

	while (args[i][++(*j)])
	{
		if (args[i][*j] == '=')
		{
		// handling empty key case (export =value)
			if (*j == 0)
			{
				print_msg(args[i]);
				break ;
			}
		// handling += case
			if (j > 0 && args[i][(*j) - 1] == '+')
			{
				if (handle_join(args[i], env, *j) == BREAK)
					break ;
			}
			// normal = case
			else
			{ // handle_normal()
			printf("test\n");
				if (handle_normal(args[i], env, *j) == BREAK)
					break ;
			printf("test1\n");
			}
			break ;
		}
	}
	return (OK);
}

int	exec_export(t_env_list **env, char **args)
{
	int			i;
	int			j;

	i = 0;
	if (!args[1]) 	
		return (print_export(*env), 0);
	while (args[++i])
	{
		j = -1;
		handle_export(args, env, i,&j);
		// No = found, just key
		printf("wejkfd\n");
		if (!args[i][j] && j > 0)
			handle_key_alone(args[i], env);
		printf("1234\n");
	}
	return (0);
}



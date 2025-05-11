#include "../includes/minishell.h"


static int	validate_key(char *s)
{
	int	i;

	i = 0;
	if (s[i] >= '0' && s[i] <= '9')
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

void	edit_env_list(t_env_list **d, char *key, char *value) // i give key, if found change value, else insert_node()
{
	t_env_list	*env;

	if (*d == NULL) // if list doesn't exist
	{
		insert_node_last(d, key, value, 0);
		return ;
	}
	env = *d;
	while (env)
	{
		if (!ft_strcmp(env->key, key))
		{
			free(env->value);
			env->value = ft_strdup(key);
			return ;
		}
		env = env->next;
	}
	insert_node_last(d, key, value, 0);
}

void	print_export(t_env_list *env_list)
{
	while (env_list)
	{
		printf("declare -x %s", env_list->key);
		if (env_list->flag)
			printf("=\"%s\"\n", env_list->value);
		else
			printf("\n");
		env_list = env_list->next;
	}
}

int	exec_export(t_env_list **env, char **args)
{
	int		i;
	int		j;
	int		flag;
	char	*key;
	char	*value;

	i = 1;
	j = 0;
	if (args[1] == NULL) 
		print_export(*env);
	else // with letter, underscore, number not at first.
	{
		while (args[i])
		{
			j = 0;
			while (args[i][j]) // either we will have 'key' | 'key=' | 'key=abc' or invalid key
			{
				if (args[i][j] == '=')
				{
					key = ft_substr(args[i], 0, j);
					if (validate_key(key))
					{
						value = ft_substr(args[i], j + 1, ft_strlen(args[i]) - j);
						flag = 1; // flag is for variable with = sign so we print them with export and not print with env
						printf("we are here");
						insert_node_last(env, key, value, flag);
						return (0);
					}
				}
				j++;
			}
			key = ft_substr(args[i], 0, j);
			if (validate_key(key))
			{
				value = ft_strdup("");
				flag = 0;
				insert_node_last(env, key, value, flag);
			}
			else\
			{
				printf("export: `%s': not a valid identifier\n", key);
				
			}
			i++;
		}
	}
	return 0;
}

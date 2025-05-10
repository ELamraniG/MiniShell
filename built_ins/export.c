#include "../includes/minishell.h"

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
static int	validate_key(char *s)
{
	int	i;

	i = 0;
	if (s[i] >= '0' && s[i] <= '9')
		return (0);
	i++;
	while (s[i])
	{
		if (s[i] != '_' || !ft_isalnum(s[i]))
			return (0);
		i++;
	}
	return (1);
}

void	get_value_env() // function i give it key, it returns the value, if no value found return NULL
{

}

void	edit_env_list() // i give key, if found change value, else insert_node()
{

}

int	exec_export(t_env_list **env, char **args)
{
	int	i;
	int	j;
	int	flag;
	char	*key;
	char	*value;

	i = 0;
	j = 0;
	if (args[1] == NULL)
		print_export(*env);
	else // with letter, underscore, number not at first
	{
		while (args[i])
		{
			j = 0;
			while (args[i][j])
			{
				if (validate_key(args[i]))
				{
					if (args[i][j] == '=')
					{
						(*env)->key = ft_substr(args[i], 0, j - 1);
						(*env)->value = ft_substr(args[i], j + 1, ft_strlen(args[i]) - j);
						flag = 1;
					}
					else
					{
						(*env)->key = ft_strdup(args[i]);
						(*env)->value = ft_strdup("");
						flag = 0;
					}
					insert_node(env, key, value, flag);
				}
				j++;
			}
			i++;
		}
	}
	return 0;
}

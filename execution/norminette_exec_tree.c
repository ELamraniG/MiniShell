#include "../includes/minishell.h"

void	dup3(int new, int original)
{
	dup2(new, original);
	close(new);
}



int	handle_path(char **args, t_env_list *env)
{
	int		i;
	char	*tmp_free;
	char	*path;
	char	**splited_path;
	char	*tmp_join;

	i = 0;
	if (!args || !args[0] || !*args[0])
		return (-1);
	if (ft_strchr(args[0], '/'))
	{
		if (!access(args[0], X_OK))
			return (1);
		if (access(args[0], F_OK) == 0)
			return (-2);
		return (-1);
	}
	env = get_env_value(env, "PATH");
	if (!env || !env->value || !env->value[0])
		return (-3);
	path = env->value;
	if (path == NULL || !*path)
		return (-1);
	splited_path = ft_split(path, ':');
	while (splited_path[i])
	{
		tmp_free = splited_path[i];
		splited_path[i] = ft_strjoin(splited_path[i], "/");
		free(tmp_free);
		tmp_join = ft_strjoin(splited_path[i], args[0]);
		if (!access(tmp_join, X_OK))
		{
			tmp_free = args[0];
			args[0] = tmp_join;
			i = 0;
			while (splited_path[i])
				free(splited_path[i++]);
			free(splited_path);
			return (1);
		}
		free(tmp_join);
		i++;
	}
	i = 0;
	while (splited_path[i])
		free(splited_path[i++]);
	free(splited_path);
	return (-1);
}



char	**turn_env_to_chars(t_env_list *env)
{
	char		**s;
	char		*tmp_free;
	t_env_list	*tmp;
	int			i;

	tmp = env;
	i = 0;
	while (env)
	{
		i++;
		env = env->next;
	}
	if (i == 0)
		return (NULL);
	s = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (tmp)
	{
		s[i] = ft_strjoin(tmp->key, "=");
		tmp_free = s[i];
		s[i] = ft_strjoin(s[i], tmp->value);
		free(tmp_free);
		i++;
		tmp = tmp->next;
	}
	s[i] = NULL;
	i = 0;
	return (s);
}

static void	handle_andd(t_ast_tree *astree, int *status, t_env_list **env)
{
	excute_the_damn_tree(astree->left, status, env);
	if (*status == 0)
		excute_the_damn_tree(astree->right, status, env);
}
static void	handle_orr(t_ast_tree *astree, int *status, t_env_list **env)
{
	excute_the_damn_tree(astree->left, status, env);
	if (*status != 0)
		excute_the_damn_tree(astree->right, status, env);
}

static void	handle_empty_command_with_redir(t_ast_tree *astree, int *status,
		t_env_list **env)
{
	int	stdinn;
	int	stdoutt;

	stdinn = dup(STDIN_FILENO);
	stdoutt = dup(STDOUT_FILENO);
	if (expand_file_name(astree, *env, *status) == -1)
	{
		*status = 1;
		return (dup3(stdoutt, STDOUT_FILENO), dup3(stdinn, STDIN_FILENO));
	}
	join_all_redir_files_without_spaces(astree);
	if (excute_redirs(astree) == -1)
	{
		dup3(stdinn, STDIN_FILENO);
		*status = 1;
		return (dup3(stdoutt, STDOUT_FILENO));
	}
	if (astree->left)
		excute_the_damn_tree(astree->left, status, env);
	else if (astree->right)
		excute_the_damn_tree(astree->right, status, env);
	else
		*status = 0;
	dup3(stdinn, STDIN_FILENO);
	dup3(stdoutt, STDOUT_FILENO);
}



void	excute_the_damn_tree(t_ast_tree *astree, int *status, t_env_list **env)
{
	int			pipes[2];
	int			exit_code;
	char		*tmp;
	int			stdinn;
	int			stdoutt;

	if (!astree)
		return ;
	if (astree->type == PIPE)
		handle_pipe(astree, pipes, status, env);
	else if (astree->type == AND)
		handle_andd(astree, status, env);
	else if (astree->type == OR)
		handle_orr(astree, status, env);
	else if (astree->type < 4)
		handle_empty_command_with_redir(astree, status, env);
	else if (astree->type == WORD)
		norminette_handle_word(astree,env,status);
}

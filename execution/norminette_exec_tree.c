#include "../includes/minishell.h"

void	dup3(int new, int original)
{
	dup2(new, original);
	close(new);
}

static int	path_norm1(char **args)
{
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
	return (0);
}

static int	norm_path2(char **splited_path, char **args, int i)
{
	char	*tmp_free;
	char	*tmp_join;

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
	return (0);
}

int	handle_path(char **args, t_env_list *env)
{
	int		i;
	char	*path;
	char	**splited_path;
	int		kk;

	i = 0;
	kk = path_norm1(args);
	if (kk != 0)
		return (kk);
	env = get_env_value(env, "PATH");
	if (!env || !env->value || !env->value[0])
		return (-3);
	path = env->value;
	if (path == NULL || !*path)
		return (-1);
	splited_path = ft_split(path, ':');
	while (splited_path[i])
	{
		if (norm_path2(splited_path, args, i++) == 1)
			return (1);
	}
	i = 0;
	while (splited_path[i])
		free(splited_path[i++]);
	return (free(splited_path), -1);
}

void normeee(t_env_list **t,char *tmp)
{
	free(tmp);
	*t = (*t)->next;
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
		i++;
		normeee(&tmp,tmp_free);
	}
	return (s[i] = NULL, s);
}

static void	handle_andd(t_ast_tree *astree, int *status, t_env_list **env,
		int in_pipe)
{
	excute_the_damn_tree(astree->left, status, env, in_pipe);
	if (*status == 0)
		excute_the_damn_tree(astree->right, status, env, in_pipe);
}

static void	handle_orr(t_ast_tree *astree, int *status, t_env_list **env,
		int in_pipe)
{
	excute_the_damn_tree(astree->left, status, env, in_pipe);
	if (*status != 0)
		excute_the_damn_tree(astree->right, status, env, in_pipe);
}

static void	handle_empty_command_with_redir(t_ast_tree *astree, int *status,
		t_env_list **env, int in_pipe)
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
		excute_the_damn_tree(astree->left, status, env, in_pipe);
	else if (astree->right)
		excute_the_damn_tree(astree->right, status, env, in_pipe);
	else
		*status = 0;
	dup3(stdinn, STDIN_FILENO);
	dup3(stdoutt, STDOUT_FILENO);
}

void	excute_the_damn_tree(t_ast_tree *astree, int *status, t_env_list **env,
		int in_pipe)
{
	int		pipes[2];
	int		exit_code;
	char	*tmp;
	int		stdinn;
	int		stdoutt;

	if (!astree)
		return ;
	if (astree->type == PIPE)
		handle_pipe(astree, pipes, status, env);
	else if (astree->type == AND)
		handle_andd(astree, status, env, in_pipe);
	else if (astree->type == OR)
		handle_orr(astree, status, env, in_pipe);
	else if (astree->type < 4)
		handle_empty_command_with_redir(astree, status, env, in_pipe);
	else if (astree->type == WORD)
	{
		if (in_pipe)
			norminette_handle_word2(astree, env, status);
		else
			norminette_handle_word1(astree, env, status);
	}
}

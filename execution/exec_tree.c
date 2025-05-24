#include "../includes/minishell.h"

char **join_args_without_spaces(t_ast_tree *astree)
{
    char **args2;
    char *tmp;
    int i, j, count;
    
    count = 1;
    i = 0;
    while (i < astree->arg_counter - 1) {
        if (astree->is_space[i])
            count++;
        i++;
    }
    if (count == astree->arg_counter)
        return astree->args;
    args2 = malloc(sizeof(char*) * (count + 1));
    
    j = 0;
    args2[0] = ft_strdup(astree->args[0]);
    i = 1;
    while (i < astree->arg_counter) {
        if (astree->is_space[i-1]) {
            j++;
            args2[j] = ft_strdup(astree->args[i]);
        }
        else {
            tmp = args2[j];
            args2[j] = ft_strjoin(tmp, astree->args[i]);
            free(tmp);
        }
        i++;
    }
    args2[count] = NULL;
    
    i = 0;
    while (i < astree->arg_counter) {
        free(astree->args[i]);
        i++;
    }
    free(astree->args);
    astree->arg_counter = count;
    return args2;
}


static char *join_files_without_spaces(t_redirect *redir)
{
    char *args2 = ft_strdup("");
    char *tmp_free;
    int i = 0;
	while(i < redir->file_str_count)
	{
		tmp_free = args2;
		args2 = ft_strjoin(args2,redir->file_name[i]);
		free(tmp_free);
		i++;
	}	
    return args2;
}
static void join_all_redir_files_without_spaces(t_ast_tree *astree)
{
	t_redirect *tmp;
	tmp = astree->redirect;
	while (tmp)
	{
		tmp->final_file_name = join_files_without_spaces(tmp);
		tmp = tmp->next;
	}
}


int handle_path(char **args, t_env_list *env)
{
	int i = 0;
	char *tmp_free;

	if (!args || !args[0] || !*args[0])
		return -1;

	if (ft_strchr(args[0],'/'))
	{
		if (!access(args[0], X_OK))
			return 1;
		if (access(args[0], F_OK) == 0)
			return -2; 
		return -1; 
	}

	env = get_env_value(env,"PATH");

	if (!env || !env->value || !env->value[0])
		return -3;
	char *path = env->value;
	if(path == NULL || !*path)
		return -1;
	char **splited_path = ft_split(path,':');

	while(splited_path[i])
	{
		tmp_free = splited_path[i];
		splited_path[i] = ft_strjoin(splited_path[i], "/");
		free(tmp_free);
		char *tmp_join = ft_strjoin(splited_path[i],args[0]);
		if (!access(tmp_join, X_OK))
		{
			tmp_free = args[0];
			args[0] = tmp_join;
			i = 0;
			while(splited_path[i])
				free(splited_path[i++]);
			free(splited_path);
			return 1;
		}
		free(tmp_join);
		i++;
	}
	i = 0;
	while(splited_path[i])
		free(splited_path[i++]);
	free(splited_path);
	return -1;
}

int is_built_in(char *cmd)
{
	char *built_ins[8]; 
	built_ins[0] = "cd";
	built_ins[1] = "echo";
	built_ins[2] = "exit";
	built_ins[3] = "env";
	built_ins[4] = "pwd";
	built_ins[5] = "unset";
	built_ins[6] = "export"; 
	built_ins[7] = NULL;    

	int i = 0;

	if (!cmd)
		return 0;

	while (built_ins[i])
	{
		if (ft_strcmp(cmd, built_ins[i]) == 0)
			return 1;
		i++;
	}
	return 0;
}

int execute_built_in(char **args, t_env_list **env)
{
	if (ft_strcmp(args[0], "cd") == 0)
		return change_dir(args, *env); 
	else if (ft_strcmp(args[0], "echo") == 0)
		return echo(args);
	else if (ft_strcmp(args[0], "exit") == 0)
		return exit_shell(args); 
	else if (ft_strcmp(args[0], "env") == 0)
		return print_env(*env); 
	else if (ft_strcmp(args[0], "pwd") == 0)
		return exec_pwd(1); 
	else if (ft_strcmp(args[0], "unset") == 0)
		return exec_unset(env, args); 
	else if (ft_strcmp(args[0], "export") == 0)
		return exec_export(env, args);
	return 1;
}

void	dup3(int new, int original)
{
	dup2(new, original);
	close(new);
}

char **turn_env_to_chars(t_env_list *env)
{
    char **s;
    char *tmp_free;
    t_env_list *tmp;
    tmp = env;
    int i = 0;
    while (env)
    {
        i++;
        env = env->next;
    }
    if (i == 0)
        return NULL;
    s = malloc(sizeof(char *) * (i + 1));
    i = 0;
    while (tmp)
    {
        s[i] = ft_strjoin(tmp->key,"=");
        tmp_free = s[i];
        s[i] = ft_strjoin(s[i],tmp->value);
        free(tmp_free);
        i++;
        tmp = tmp->next;
    }
    s[i] = NULL;
    i = 0;
    return s;
}



void	print_tree(t_ast_tree *tree, int deep);
void excute_the_damn_tree(t_ast_tree *astree, int *status, t_env_list **env)
{
	int			pipes[2];
	int			pid1;
	int			pid2;
	int			exit_code;
	struct stat	l;
	char		*tmp;
	int			stdinn;
	int			stdoutt;

	if (!astree)
		return ;
	if (astree->type == PIPE)
	{
		if (pipe(pipes) == -1)
		{
			*status = 1;
			perror(NULL);
			exit(106) ;
		}
		pid1 = fork();
		if (pid1 == -1)
		{
			close(pipes[0]);
			close(pipes[1]);
			*status = 1;
			perror(NULL);
			exit(106) ;
		}
		if (pid1 == 0)
		{
			dup3(pipes[1], STDOUT_FILENO);
			close(pipes[0]);
			excute_the_damn_tree(astree->left, status, env);
			exit(*status);
		}
		pid2 = fork();
		if (pid2 == -1)
		{
			close(pipes[0]);
			close(pipes[1]);
			*status = 1;
			perror(NULL);
			waitpid(pid1, NULL, 0);
			exit(106) ;
		}
		if (pid2 == 0)
		{
			dup3(pipes[0], STDIN_FILENO);
			close(pipes[1]);
			excute_the_damn_tree(astree->right, status, env);
			exit(*status);
		}
		close(pipes[0]);
		close(pipes[1]);
		ignore_signals();
		waitpid(pid1, NULL, 0);
		waitpid(pid2, &exit_code, 0);
		handle_main_sigs();
		if (WIFEXITED(exit_code))
			*status = WEXITSTATUS(exit_code);
		else if (WIFSIGNALED(exit_code))
			*status = 128 + WTERMSIG(exit_code);
	}
	else if (astree->type == AND)
	{
		excute_the_damn_tree(astree->left, status, env);
		if (*status == 0)
			excute_the_damn_tree(astree->right, status, env);
	}
	else if (astree->type == OR)
	{
		excute_the_damn_tree(astree->left, status, env);
		if (*status != 0)
			excute_the_damn_tree(astree->right, status, env);
	}
    else if (astree->type < 4)
    {
        stdinn = dup(STDIN_FILENO);
        stdoutt = dup(STDOUT_FILENO);
        
        if (excute_redirs(astree) == -1) 
        {
            dup3(stdinn, STDIN_FILENO);
            dup3(stdoutt, STDOUT_FILENO);
            *status = 1;
            return;
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
	else if (astree->type == WORD)
	{
		stdinn = dup(STDIN_FILENO);
		stdoutt = dup(STDOUT_FILENO);
		
		I_HATE_EXPANDING(astree, *env, *status);
		if (expand_file_name(astree,*env, *status) == -1)
		{
			*status = 1;
			dup3(stdinn, STDIN_FILENO);
			dup3(stdoutt, STDOUT_FILENO);
			return;
		}
		astree->args = join_args_without_spaces(astree);
		join_all_redir_files_without_spaces(astree);
		handle_wild_card(astree);
		if (excute_redirs(astree) == -1)
		{
			dup3(stdinn, STDIN_FILENO);
			dup3(stdoutt, STDOUT_FILENO);
			*status = 1;
			return;
		}
		if (is_built_in(astree->args[0]))
		{
			*status = execute_built_in(astree->args, env);
			dup3(stdinn, STDIN_FILENO);
			dup3(stdoutt, STDOUT_FILENO);
			return;
		}
		pid1 = fork();
		if (pid1 == -1)
		{
			close(stdinn);
			close(stdoutt);
			*status = 1;
			perror(NULL);
			exit(106);
		}
		if (pid1 == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);

			struct stat l;
			//i used pid just for norminette
			pid2 = handle_path(astree->args, *env);
			if (pid2 == -1)
			{
				ft_putstr_fd(2, astree->args[0]);
				ft_putstr_fd(2, ": command not found\n");
				exit(127);
			}
			else if (pid2 == -2)
			{
				ft_putstr_fd(2, astree->args[0]);
				ft_putstr_fd(2, ": Permission denied\n"); // check this later
				exit(126);
			}
			else if (pid2 == -3)
			{
				ft_putstr_fd(2, "minishell: ");
				ft_putstr_fd(2, astree->args[0]);
				ft_putstr_fd(2, ": No such file or directory\n");
				exit(127);
			}
			if (stat(astree->args[0], &l) == 0 && S_ISDIR(l.st_mode))
			{
				ft_putstr_fd(2, astree->args[0]);
				ft_putstr_fd(2, ": is a directory\n");
				exit(126);
			}
			char **env_char = turn_env_to_chars(*env);
			execve(astree->args[0], astree->args, env_char);
			perror(astree->args[0]);
			if (env_char)
			{
				//again,,,pid just for norminette
				pid2 = 0;
				while (env_char[pid2])
					free(env_char[pid2++]);
				free(env_char);
			}
			exit(127);
		}
		ignore_signals();
		waitpid(pid1, &exit_code, 0);
		handle_main_sigs();
		if (WIFEXITED(exit_code))
			*status = WEXITSTATUS(exit_code);
		else if (WIFSIGNALED(exit_code))
		{
			*status = 128 + WTERMSIG(exit_code);
			if (WTERMSIG(exit_code) == SIGQUIT)
				ft_putstr_fd(2, "Quit: 3\n");
			ft_putstr_fd(2, "\n");
		}
		dup3(stdinn, STDIN_FILENO);
		dup3(stdoutt, STDOUT_FILENO);
	}
}


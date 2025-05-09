#include "../includes/minishell.h"

// later on msa7 argcounter mmen list
//

// madnsachi FREE f CASE DYALL ERRORS bhal unclosed quote W CTRL D KBEL MADOZ N EXEC
// status f exit dyal syntax err khass men baed tfixiha
// quotes fihom mochkel when using "" or '' fix them later "" and '' gives NULL
// you should add to history everything even the syntax errors
//$ $dd
// handle this shit later in exec
/*$ "$Dd
> bash: unexpected EOF while looking for matching `"'
bash: syntax error: unexpected end of file
EL | Amrani --> : ~/Desktop/1733/MiniShell
$ "$Dd"
bash: : command not found
EL | Amrani --> : ~/Desktop/1733/MiniShell
$
*/

void	print_lex(t_lex_list *temp)
{
	while (temp)
	{
		printf("Token: |%s|, Type: %d\n", temp->s, temp->a_type);
		temp = temp->next;
	}
}

void	print_tree(t_ast_tree *tree, int deep)
{
	int			i;
	t_redirect	*t;

	if (!tree)
		return ;
	for (int i = 0; i < deep; i++)
		printf(" ");
	if (tree->args)
	{
		i = 0;
		while (tree->args[i])
		{
			printf(" |%s quote = %d is_space = %d|  ", tree->args[i],
				tree->q_type[i], tree->is_space[i]);
			i++;
		}
		printf("\n");
	}
	else
		printf("%d\n", tree->type);
	t = tree->redirect;
	while (t)
	{
		printf("|%s| |%d| ", t->file_name, t->type);
		t = t->next;
	}
	print_tree(tree->left, deep + 1);
	print_tree(tree->right, deep + 1);
}
void	free_reds(t_redirect *red)
{
	t_redirect	*tmp;

	tmp = NULL;
	while (red)
	{
		tmp = red;
		red = red->next;
		free(tmp->file_name);
		free(tmp);
	}
}

void	free_args(t_ast_tree *root)
{
	int	i;

	i = 0;
	while (root->args[i])
	{
		free(root->args[i]);
		i++;
	}
}

void	free_tree(t_ast_tree *root)
{
	if (!root)
		return ;
	if (root->redirect)
		free_reds(root->redirect);
	if (root->args)
	{
		free_args(root);
		free(root->args);
		free(root->is_space);
		free(root->q_type);
	}
	free_tree(root->left);
	free_tree(root->right);
	free(root);
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
				return -1;
			}
			
			env = get_env_value(env,"PATH");
			
			if (!env || !env->value || !env->value[0])
				return -1;
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
			if (!access(args[0], X_OK))
			{
				i = 0;
				while(splited_path[i])
					free(splited_path[i++]);
				free(splited_path);
				return 1;
			}
			i = 0;
					while(splited_path[i])
						free(splited_path[i++]);
					free(splited_path);
			return -1;
		}
void	dup3(int new, int original)
{
	dup2(new, original);
	close(new);
}


void	excute_the_damn_tree(t_ast_tree *astree, int *status, t_env_list *env)
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
			return ;
		}
		pid1 = fork();
		if (pid1 == -1)
		{
			close(pipes[0]);
			close(pipes[1]);
			*status = 1;
			perror(NULL);
			return ;
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
			return ;
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
		waitpid(pid1, NULL, 0);
		waitpid(pid2, &exit_code, 0);
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
	else if (astree->type == WORD)
	{
		stdinn = dup(STDIN_FILENO);
		stdoutt = dup(STDOUT_FILENO);
		if (excute_redirs(astree) == -1)
		{
			close(stdinn);
			close(stdoutt);
			*status = 1;
			return ;
		}
		// if (is_built_in(astree->args))
		//
		pid1 = fork();
		if (pid1 == -1)
		{
			close(stdinn);
			close(stdoutt);
			*status = 1;
			perror(NULL);
			return ;
		}
		if (pid1 == 0)
		{
			stat(astree->args[0], &l);
			if (S_ISDIR(l.st_mode))
			{
				ft_putstr_fd(2, astree->args[0]);
				ft_putstr_fd(2, ": is a directory\n");
				exit(126);
			}
			if (handle_path(astree->args, env) == -1)
			{
				ft_putstr_fd(2,astree->args[0]);
				ft_putstr_fd(2, ": command not found");
				exit(127);	
			}
			execve(astree->args[0], astree->args, NULL);
			perror(NULL);
			exit(127);
		}
		waitpid(pid1, &exit_code, 0);
		if (WIFEXITED(exit_code))
			*status = WEXITSTATUS(exit_code);
		else if (WIFSIGNALED(exit_code))
			*status = 128 + WTERMSIG(exit_code);
		dup3(stdinn, STDIN_FILENO);
		dup3(stdoutt, STDOUT_FILENO);
	}
}

int	main(int ac, char **av, char **env)
{
	char		*input;
	t_lex_list	*tokens;
	int			status;
	t_ast_tree	*astree;
	t_lex_list	*lopo;

	t_env_list *envv = NULL; 
	set_up_env(env, &envv);

	ac = 0;
	av = NULL;
	// env = NULL;
	status = 0;
	astree = NULL;
	while (1)
	{
		status = 0;
		input = readline("minishell$ ");
		if (!input)
			break ;
		if (input[0])
			add_history(input);
		tokens = lexing_the_thing(input, &status);
		if (status != 0)
		{
			printf("status = %d\n", status);
			free(input);
			free_lex_list(tokens);
			continue ;
		}
		set_the_arg_type(tokens);
		lopo = tokens;
		handle_syntax_errors(tokens, &status);
		if (status != 0)
		{
			printf("status = %d\n", status);
			free(input);
			free_lex_list(tokens);
			continue ;
		}
		astree = create_ast_tree(tokens);
		remove_quotes(tokens);
		free_lex_list(tokens);
		excute_the_damn_tree(astree, &status, envv);
		free_tree(astree);
		printf("status = %d\n", status);
		free(input);
		if (!isatty(STDIN_FILENO)) {
			free_env_list(envv);
			return status;
		}
	}
	free_env_list(envv);	
return (0);
}
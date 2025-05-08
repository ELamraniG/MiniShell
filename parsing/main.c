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
		printf("Token: '%s', Type: %d\n", temp->s, temp->a_type);
		temp = temp->next;
	}
}

void	lopo(void)
{
	system("leaks minishell");
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

void	dup3(int new, int original)
{
	dup2(new, original);
	close(new);
}
void	excute_the_damn_tree(t_ast_tree *astree, int *status, char **env)
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
				ft_putstr_fd(2, "./parsing: is a directory\n");
				exit(126);
			}
			tmp = astree->args[0];
			astree->args[0] = ft_strjoin("/bin/", astree->args[0]);
			free(tmp);
			execve(astree->args[0], astree->args, env);
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

	ac = 0;
	av = NULL;
	// env = NULL;
	status = 0;
	astree = NULL;
	// atexit(lopo);
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
		excute_the_damn_tree(astree, &status, env);
		free_tree(astree);
		printf("status = %d\n", status);
		free(input);
	}
	return (0);
}

// int main(int ac, char **av, char **env)
// this main is to test insert node and remove node
// {
// 	t_env_list	*env_list = NULL;
// 	t_env_list	*tmp;
// 	(void)ac;
// 	(void)av;

// 	int i = 0;
// 	set_up_env(env, &env_list);
// 	tmp = env_list;
// 	while (tmp)
// 	{
// 		printf("%d | ", i);
// 		printf("%s=", tmp->key);
// 		printf("%s\n", tmp->value);
// 		tmp = tmp->next;
// 		i++;
// 	}
// 	tmp = env_list;
// 	printf("\n-----------------\n");
// 	delete_node(&env_list, "TERM");
// 	tmp = env_list;
// 	delete_node(&env_list, "USER_ZDOTDIR");
// 	printf("-----------------\n");
// 	i = 0;
// 	while (env_list)
// 	{
// 		printf("%d | ", i);
// 		printf("%s=", env_list->key);
// 		printf("%s\n", env_list->value);
// 		env_list = env_list->next;
// 		i++;
// 	}
// }

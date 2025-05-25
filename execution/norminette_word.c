#include "../includes/minishell.h"

static int word_norm(t_ast_tree *astree,t_env_list **env, int *status, t_stdindo stddd)
{
	I_HATE_EXPANDING(astree, *env, *status);
		if (expand_file_name(astree, *env, *status) == -1)
		{
			*status = 1;
			dup3(stddd.stdinn, STDIN_FILENO);
			dup3(stddd.stdoutt, STDOUT_FILENO);
			return -1;
		}
		handle_wild_card(astree);
		if (handle_file_wildcard(astree) == -1)
		{
			*status = 1;
			dup3(stddd.stdinn, STDIN_FILENO);
			dup3(stddd.stdoutt, STDOUT_FILENO);
			return -1;
		}
		astree->args = join_args_without_spaces(astree);
		join_all_redir_files_without_spaces(astree);
		return (0);
}

static int word_norm2(t_ast_tree *astree,t_stdindo stddd,int *status,t_env_list **env)
{
		if (excute_redirs(astree) == -1)
		{
			dup3(stddd.stdinn, STDIN_FILENO);
			dup3(stddd.stdoutt, STDOUT_FILENO);
			*status = 1;
			return -1;
		}
		if (is_built_in(astree->args[0]))
		{
			*status = execute_built_in(astree->args, env, status);
			dup3(stddd.stdinn, STDIN_FILENO);
			dup3(stddd.stdoutt, STDOUT_FILENO);
			return -1;
		}
		return (0);
}
void norminette_handle_word(t_ast_tree *astree,t_env_list **env,int *status)
{
	t_stdindo stddd;
	int pid1;
	int exit_code;
	char		**env_char;
	struct stat	l;

		stddd.stdinn = dup(STDIN_FILENO);
		stddd.stdoutt = dup(STDOUT_FILENO);
		if (word_norm(astree,env,status,stddd) == -1 || word_norm2(astree,stddd,status,env) == -1)
			return ;
		pid1 = fork();
		if (pid1 == -1)
		{
			close(stddd.stdinn);
			close(stddd.stdoutt);
			*status = 1;
			perror(NULL);
			exit(106);
		}
		if (pid1 == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			exit_code = handle_path(astree->args, *env);
			if (exit_code == -1)
			{
				ft_putstr_fd(2, astree->args[0]);
				ft_putstr_fd(2, ": command not found\n");
				exit(127);
			}
			else if (exit_code == -2)
			{
				ft_putstr_fd(2, astree->args[0]);
				ft_putstr_fd(2, ": Permission denied\n");
				exit(126);
			}
			else if (exit_code == -3)
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
			env_char = turn_env_to_chars(*env);
			execve(astree->args[0], astree->args, env_char);
			perror(astree->args[0]);
			if (env_char)
			{
				exit_code = 0;
				while (env_char[exit_code])
					free(env_char[exit_code++]);
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
			if (WTERMSIG(exit_code) == SIGINT)
				ft_putstr_fd(2, "\n");
		}
		dup3(stddd.stdinn, STDIN_FILENO);
		dup3(stddd.stdoutt, STDOUT_FILENO);
	}
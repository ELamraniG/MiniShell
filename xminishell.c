#include "includes/minishell.h"
int gstatus = 0;

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

void handle_signal(int n)
{
	if (n == SIGINT)
		gstatus = 130;
	else if (n == SIGQUIT)
		gstatus = 131;
}

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
	int i = 0;
	status = 0;
	astree = NULL;
	signal(SIGINT,handle_signal);
	signal(SIGQUIT,handle_signal);
	while (1)
	{
		status = 0;
		input = readline("minishell$ ");
		if (gstatus == 130)
		{
			status = 130;
			gstatus = 0;
			rl_on_new_line();
			rl_replace_line("",0);
			rl_redisplay();
			continue;
		}
		if(gstatus == 131)
		{
			gstatus = 0;
			status = 131;
		}
		i++;
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
		excute_the_damn_tree(astree, &status, envv,i);
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
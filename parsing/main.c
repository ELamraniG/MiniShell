#include "../includes/minishell.h"



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

void print_tree(t_ast_tree *tree, int deep)
{
	if (!tree)
		return ;
	for (int i = 0; i < deep; i++)
		printf(" ");
	if (tree->args)
	{
		int  i = 0;
		while (tree->args[i])
		{
			printf(" |%s quote = %d is_space = %d|  ",tree->args[i],tree->q_type[i],tree->is_space[i]);
			i++;
		}
		printf("\n");
	}
	else
		printf("%d\n",tree->type);
	print_tree(tree->left, deep + 1);
	print_tree(tree->right, deep + 1);
}
void free_reds(t_redirect *red)
{
	t_redirect *tmp = NULL;
	while(red)
	{
		tmp = red;
		red = red->next;
		free(tmp);
	}
}

void free_args(t_ast_tree *root)
{
	int i = 0 ;
	while (root->args[i])
	{
		free(root->args[i]);
		i++;
	}
	printf("\n");
}

void free_tree(t_ast_tree *root)
{
	if (!root)
		return;
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
// int	main(void)
// {
// 	char *input;
// 	t_lex_list *tokens;
// 	int status = 0;
// 	t_ast_tree *exec_tree = NULL;
// 	atexit(lopo);
// 	while (1)
// 	{
// 		status = 0;
// 		input = readline("\033[32mminishell$ \033[0m");
// 		if (!input)
// 			break ;
// 		if (input[0])
// 			add_history(input);
// 		tokens = lexing_the_thing(input, &status);
// 		if (status != 0)
// 		{
// 			free(input);
// 			free_lex_list(tokens);
// 			continue ;
// 		}
// 		set_the_arg_type(tokens);
// 		handle_syntax_errors(tokens, &status);
// 		if (status != 0)
// 		{
// 			free(input);
// 			free_lex_list(tokens);
// 			continue ;
// 		}
// 		remove_quotes(tokens);
// 		exec_tree = create_ast_tree(tokens);
// 		print_tree(exec_tree, 0);
// 		free_tree(exec_tree);
// 		print_lex(tokens);
// 		free_lex_list(tokens);
// 		free(input);
// 	}

// 	return (0);
// }


int main(int ac, char **av, char **env)
{
	t_env_list	*env_list = NULL;
	(void)ac;
	(void)av;

	set_up_env(env, &env_list);
	while (env_list)
	{
		printf("%s", env_list->key);
		printf("%s\n", env_list->value);
		env_list = env_list->next;
	}
}
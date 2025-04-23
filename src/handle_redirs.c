#include "../includes/minishell.h"
static int is_special_for_redirs(int a)
{
	if (a == WORD && a < 4)
		return 1;
	return 0;
}


void handle_redirs(t_ast_tree *node, t_lex_list **token)
{
	t_redirect *tmp = NULL;
	while (*token && (*token)->a_type < 4)
	{
		add_to_list_redir(&tmp,(*token)->next->s,(*token)->a_type);
		*token = (*token)->next->next;
	}
	node->redirect = tmp;
}
void	handle_word(t_ast_tree *node, t_lex_list **token)
{
	char		**args;
	int			malc;
	t_lex_list	*tmp;
	int			i;

	malc = 0;
	tmp = *token;
	// if (*token && (*token)->next && ((*token)->next)->a_type != WORD)
	// {
	// 	args = malloc(sizeof(char));
	// 	args[0] = 0;
	// 	node->args = args;
	// 	*token = (*token)->next;
	// 	return ;
	// }
	while (tmp && (tmp)->a_type == WORD)
	{
		malc++;
		tmp = tmp->next;
	}
	args = malloc(sizeof(char *) * malc + 1);
	args[malc] = 0;
	i = 0;
	while (*token && (*token)->a_type == WORD)
	{
		args[i] = (*token)->s;
			//// BIG PROBLEM BIG PROBLEM    WE ASSIGN THE SAME STRING SO IF WE FREE THE LIST THEN THE TREE WE WILL HAVE DOUBLE FREE
		*token = (*token)->next;
		i++;
	}
	node->args = args;
	handle_redirs(node,token);
}

// int calc_size(t_lex_list *token, t_lex_list *finish)
// {
// 	int n = 0;
// 	while (token != finish)
// 	{
// 		n++;
// 		token = token->next;
// 	}
// 	return (n);
// }

// void copy_all(t_lex_list *token, t_lex_list *finish)
// {
// 	int n = calc_size(token,finish);

// 	while (token != finish)
// 	{

// 	}
// }

// t_lex_list *handle_commands(t_lex_list *token)
// {
// 	t_lex_list *finish = token;
// 	while (finish && !is_special_opperand(finish->a_type))
// 	{
// 		finish = finish->next;
// 	}
// 	copy_all(token,finish);
// }
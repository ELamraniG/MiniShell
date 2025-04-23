#include "../includes/minishell.h"

t_ast_tree	*new_ast_node(t_lex_list *node)
{
	t_ast_tree	*tree_node;

	tree_node = malloc(sizeof(t_ast_tree));
	tree_node->content = node->s;
	return (tree_node);
}
t_ast_tree *parse_and_or(t_lex_list **token)
{
	t_ast_tree *left = parse_pipe(token);
	while (*token && ((*token)->a_type == AND || (*token)->a_type == OR))
	{
		t_ast_tree *root = new_ast_node(*token);
		root->type = (*token)->a_type;
		(*token) = (*token)->next;
		t_ast_tree *right = parse_pipe(token);
		root->left = left;
		root->right = right;
		left = root;
	}
	return (left);
}

t_ast_tree *parse_pipe(t_lex_list **token)
{
	t_ast_tree *left = parse_parenthesis_cmds(token);
	while (*token && (*token)->a_type == PIPE)
	{
		t_ast_tree *root = new_ast_node(*token);
		root->type = (*token)->a_type;
		(*token) = (*token)->next;
		t_ast_tree *right = parse_parenthesis_cmds(token);
		root->left = left;
		root->right = right;
		left = root;
	}
	return (left);
}

t_ast_tree *parse_parenthesis_cmds(t_lex_list **token)
{
	if (*token == NULL)
		return NULL;
	if ((*token)->a_type == WORD)
	{
		t_ast_tree *root = new_ast_node(*token);
		root->type = (*token)->a_type;
		root->left = NULL;
		root->right = NULL;
		handle_word(root,token);
		// *token = (*token)->next;
		return (root);
	}
	if (*token && ((*token)->a_type == OP_PAREN))
	{
		*token = (*token)->next;
		t_ast_tree *root = parse_and_or(token);
		if (*token == NULL || (*token)->a_type != CL_PAREN)
			return root;
		*token = (*token)->next;
		return root;
	}
	return NULL;
}


t_ast_tree *create_ast_tree(t_lex_list *token)
{
	return (parse_and_or(&token));
}
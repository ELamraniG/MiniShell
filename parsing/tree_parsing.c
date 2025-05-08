/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-amr <moel-amr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:14:08 by moel-amr          #+#    #+#             */
/*   Updated: 2025/05/08 16:35:41 by moel-amr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_ast_tree	*new_ast_node(void)
{
	t_ast_tree	*tree_node;

	tree_node = malloc(sizeof(t_ast_tree));
	tree_node->args = NULL;
	tree_node->redirect = NULL;
	tree_node->left = NULL;
	tree_node->right = NULL;
	tree_node->q_type = NULL;
	tree_node->is_space = NULL;
	return (tree_node);
}

t_ast_tree	*parse_and_or(t_lex_list **token)
{
	t_ast_tree	*left;
	t_ast_tree	*root;
	t_ast_tree	*right;

	left = parse_pipe(token);
	while (*token && ((*token)->a_type == AND || (*token)->a_type == OR))
	{
		root = new_ast_node();
		root->type = (*token)->a_type;
		(*token) = (*token)->next;
		right = parse_pipe(token);
		root->left = left;
		root->right = right;
		left = root;
	}
	return (left);
}

t_ast_tree	*parse_pipe(t_lex_list **token)
{
	t_ast_tree	*left;
	t_ast_tree	*root;
	t_ast_tree	*right;

	left = parse_parenthesis_cmds(token);
	while (*token && (*token)->a_type == PIPE)
	{
		root = new_ast_node();
		root->type = (*token)->a_type;
		(*token) = (*token)->next;
		right = parse_parenthesis_cmds(token);
		root->left = left;
		root->right = right;
		left = root;
	}
	return (left);
}

t_ast_tree	*parse_parenthesis_cmds(t_lex_list **token)
{
	t_ast_tree	*root;

	if (*token == NULL)
		return (NULL);
	if ((*token)->a_type < 4)
	{
		root = new_ast_node();
		root->type = (*token)->a_type;
		handle_words(root, token);
		return (root);
	}
	if ((*token)->a_type == WORD)
	{
		root = new_ast_node();
		root->type = (*token)->a_type;
		handle_words(root, token);
		return (root);
	}
	if (*token && ((*token)->a_type == OP_PAREN))
	{
		*token = (*token)->next;
		root = parse_and_or(token);
		if (*token == NULL)
			return (root);
		*token = (*token)->next;
		return (root);
	}
	return (NULL);
}

t_ast_tree	*create_ast_tree(t_lex_list *token)
{
	return (parse_and_or(&token));
}

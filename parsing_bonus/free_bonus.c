/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 21:34:15 by jhamdaou          #+#    #+#             */
/*   Updated: 2025/05/29 23:11:33 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minishell_bonus.h"

void	free_lex_list(t_lex_list *token)
{
	t_lex_list	*tmp;

	while (token)
	{
		tmp = token;
		free(token->s);
		token = token->next;
		free(tmp);
	}
}

void	free_reds(t_redirect *red)
{
	int			i;
	t_redirect	*tmp;

	while (red)
	{
		tmp = red;
		i = 0;
		while (i < red->file_str_count)
		{
			free(red->file_name[i]);
			i++;
		}
		free(red->file_name);
		free(red->q_types);
		free(red->is_space);
		free(red->final_file_name);
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-amr <moel-amr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:16:40 by moel-amr          #+#    #+#             */
/*   Updated: 2025/05/13 13:24:49 by moel-amr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_special_for_redirs(int a)
{
	if (a == WORD || a < 4)
		return (1);
	return (0);
}

// static int wach_skip(t_lex_list *token)
// {
//     if (!token || !token->next)
//         return (0);
//     if (!token->is_space && token->next->a_type == WORD)
//         return (1);
//     return (0);
// }

// static int	calculate_words(t_lex_list *token)
// {
// 	int	n;
// 	t_lex_list *t;

// 	n = 0;
// 	while (token && is_special_for_redirs(token->a_type))
// 	{
// 		if (token->a_type == WORD)
// 		{
// 			n++;
// 			token = token->next;
// 		}
// 		else if (token->a_type < 4)
// 		{
// 			token = token->next;
// 			t = token;
// 			token = token->next;
// 			while (wach_skip(t))
// 			{
// 				t = token;
// 				token = token->next;
// 			}
// 		}
// 	}
// 	return (n);
// }

// static int	calculate_parts(t_lex_list *token)
// {
// 	int	count;
// 	t_lex_list *current;

// 	count = 1;  // Start with 1 for the first token
// 	current = token;

// 	while (wach_skip(current))
// 	{
// 		count++;
// 		current = current->next;
// 	}

// 	return (count);
// }

// static void collect_redirect_parts(t_lex_list **token, t_redirect *redir)
// {
//     int i = 0;
//     t_lex_list *current;

//     int parts = calculate_parts(*token);
//     redir->file_str_count = parts;
//     redir->file_name = malloc(sizeof(char *) * parts);
//     redir->q_types = malloc(sizeof(int) * parts);
//     redir->is_space = malloc(sizeof(int) * parts);

//     redir->file_name[i] = ft_strdup((*token)->s);
//     redir->q_types[i] = (*token)->q_type;
//     redir->is_space[i] = (*token)->is_space;
//     i++;

//     current = *token;
//     *token = (*token)->next;

//     while (i < parts && wach_skip(current))
//     {
//         redir->file_name[i] = ft_strdup((*token)->s);
//         redir->q_types[i] = (*token)->q_type;
//         redir->is_space[i] = (*token)->is_space;
//         i++;

//         current = *token;
//         *token = (*token)->next;
//     }
// }

// static void add_to_list_redir_with_parts(t_redirect **ll, int type,
// t_lex_list **token)
// {
//     t_redirect *new_node;
//     t_redirect *tmp;

//     new_node = malloc(sizeof(t_redirect));
//     new_node->type = type;
//     new_node->next = NULL;
//     new_node->heredoc = 0;
//     new_node->LAST_DAMN_FILE_NAME = NULL;
// Initialize to NULL to prevent segfaults

//     collect_redirect_parts(token, new_node);

//     if (*ll == NULL)
//         *ll = new_node;
//     else
//     {
//         tmp = *ll;
//         while (tmp->next)
//             tmp = tmp->next;
//         tmp->next = new_node;
//     }
// }
static int	calculate_redir_args(t_lex_list *token)
{
	int	count;

	count = 0;
	while ((token) && (token)->a_type == WORD && (token)->is_space == 0)
	{
		(token) = (token)->next;
		count++;
	}
	return (count);
}

static void	add_to_list_redir_with_parts(t_redirect **redirs, int r_type,
		t_lex_list **token)
{
	t_redirect	*red;
	int			count;
	int			i;

	red = malloc(sizeof(t_redirect));
	red->type = r_type;
	red->LAST_DAMN_FILE_NAME = NULL;
	red->heredoc = -1;
	red->next = NULL;
	count = calculate_redir_args(*token);
	red->file_str_count = count;
	red->q_types = malloc(sizeof(count) * sizeof(int));
	red->is_space = malloc(sizeof(count) * sizeof(int));
	red->file_name = malloc(sizeof(char *) * count);
	i = 0;
	while (i < count)
	{
		red->file_name[i] = ft_strdup((*token)->s);
		(*token) = (*token)->next;
		i++;
	}
	add_to_list_redir(redirs, red);
}

static void	skip_all_for_redirs(t_lex_list **token)
{
	while ((*token) && (*token)->a_type == WORD && (*token)->is_space == 0)
	{
		(*token) = (*token)->next;
	}
}

static int	calculate_words(t_lex_list *token)
{
	int	count;

	count = 0;
	while (token && is_special_for_redirs(token->a_type))
	{
		if (token->a_type < 4)
		{
			token = token->next;
			skip_all_for_redirs(&token);
			continue ;
		}
		count++;
		token = token->next;
	}
	return (count);
}

void	handle_words(t_ast_tree *node, t_lex_list **token)
{
	char		**args;
	int			malc;
	t_redirect	*getting_redirs;
	int			i;
	int			*q_type;
	int			*is_space;
	t_lex_list	*lex_tmp;
	int			r_type;

	q_type = NULL;
	is_space = NULL;
	i = 0;
	getting_redirs = NULL;
	lex_tmp = *token;
	malc = calculate_words(lex_tmp);
	args = malloc(sizeof(char *) * (malc + 1));
	q_type = malloc(sizeof(int) * (malc));
	is_space = malloc(sizeof(int) * (malc));
	node->arg_counter = malc;
	args[malc] = 0;
	while (*token && is_special_for_redirs((*token)->a_type))
	{
		if ((*token)->a_type == WORD)
		{
			args[i] = ft_strdup((*token)->s);
			q_type[i] = (*token)->q_type;
			is_space[i] = (*token)->is_space;
			i++;
			(*token) = (*token)->next;
		}
		else if ((*token)->a_type < 4)
		{
			
			r_type = (*token)->a_type;
			(*token) = (*token)->next;
			add_to_list_redir_with_parts(&getting_redirs, r_type, token);
		}
	}
	node->is_space = is_space;
	node->q_type = q_type;
	node->args = args;
	node->redirect = getting_redirs;
}

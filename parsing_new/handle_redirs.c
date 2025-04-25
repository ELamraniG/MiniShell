# include "../includes/minishell.h"

static int	is_special_for_redirs(int a)
{
	if (a == WORD || a < 4)
		return (1);
	return (0);
}

static int	calculate_words(t_lex_list *token)
{
	int	n;

	n = 0;
	while (token && is_special_for_redirs(token->a_type))
	{
		if (token->a_type == WORD)
			n++;
		if (token->a_type > 3) // all tokens mgher redirs khsnna mchiw bwahd,f redir ghanchiw bjoj bach nskipiw lword le mor redir
			token = token->next;
		else if (token->a_type < 4)
			token = token->next->next;
	}
	return (n);
}

void	handle_words(t_ast_tree *node, t_lex_list **token)
{
	char		**args;
	int			malc;
	t_redirect	*tmp;
	int			i;

	i = 0;
	malc = 0;
	tmp = NULL;
	malc = calculate_words(*token);
	args = malloc(sizeof(char *) * (malc + 1));
	args[malc] = 0;
	while (*token && is_special_for_redirs((*token)->a_type))
	{
		if ((*token)->a_type == WORD)
		{
			args[i] = (*token)->s;
				//////////// WARNING WARNING WARNING THIS COULD CAUSE BIIIG NIGGER PROBLEM,WHEN WE TRYING TO FREE LISTS WE WILL END UP DOING DOUBLE FREE BECAUSE THEY SHARE THE SAME POINTER,MENBA3D EMEL STRDUP
			i++;
			(*token) = (*token)->next;
		}
		// handle_word(node,token,args);
		else if ((*token)->a_type < 4)
		{
			add_to_list_redir(&tmp, (*token)->next->s, (*token)->a_type);
			(*token) = (*token)->next->next;
		}
	}
	node->args = args;
	node->redirect = tmp;
}

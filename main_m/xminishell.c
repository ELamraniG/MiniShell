/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xminishell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhamdaou <jhamdaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 21:50:02 by moel-amr          #+#    #+#             */
/*   Updated: 2025/05/29 22:43:13 by jhamdaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int			g_sigarette = 0;

void	norm_tre_tok(t_lex_list **tokens, t_ast_tree **astree)
{
	remove_quotes(*tokens);
	*astree = create_ast_tree(*tokens);
	free_lex_list(*tokens);
}

void	norm_sig(int *status)
{
	*status = g_sigarette;
	g_sigarette = 0;
}

void	norm_sig12(int *status)
{
	*status = 1;
	g_sigarette = 0;
}

int	handle_herdc_norm(t_final_struct *aaa)
{
	if (handle_heredoc(aaa->astree, 0, aaa->envv) == -1)
	{
		if (g_sigarette == 130)
			norm_sig12(&aaa->mainn.status);
		free(aaa->input);
		free_tree(aaa->astree);
		return (-1);
	}
	return (0);
}

int	main(int ac, char **av, char **env)
{
	t_final_struct	aaa;

	init_norm(&aaa.mainn, ac, av, &aaa.astree);
	set_up_env(env, &aaa.envv);
	while (1337)
	{
		is_tty(&aaa.input);
		if (g_sigarette != 0)
			norm_sig(&aaa.mainn.status);
		if (!aaa.input)
			break ;
		if (aaa.input[0])
			add_history(aaa.input);
		if (handle_token(&aaa.tokens, &aaa.input, &aaa.mainn.status) == -1)
			continue ;
		if (!syntax_err_normted(&aaa.tokens, &aaa.mainn.status, &aaa.input))
			continue ;
		norm_tre_tok(&aaa.tokens, &aaa.astree);
		if (handle_herdc_norm(&aaa) == -1)
			continue ;
		exec_tree_ndrest(&aaa.astree, &aaa.mainn.status, &aaa.envv, &aaa.input);
		if (!isatty(STDIN_FILENO))
			return (is_tt2(&aaa.envv), aaa.mainn.status);
	}
	return (is_tt2(&aaa.envv), aaa.mainn.status);
}

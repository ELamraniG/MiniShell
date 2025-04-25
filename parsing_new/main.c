# include "../includes/minishell.h"

int main(int argc, char **argv)
{
	char		*input;
	t_lex_list	*tokens;
	int status = 0;

	(void)argc;
	(void)argv;

	while (1)
	{
		status = 0;
		input = readline("\033[32mminishell$ \033[0m");
		if (!input) // deksaea emel free w leibat
			break;
			
		if (*input)
			add_history(input);
			
		tokens = lexing_the_thing(input, &status);
		if(status != 0)
		{
			// free(input);
			// free_list(tokens);
			continue;
		}
		
		// Make sure token types are set properly
		set_the_arg_type(tokens);
		
		handle_syntax_errors(tokens, &status);
		if (status != 0)
		{
			// free(input);
			// free_list(&tokens);
			continue;
		}
		
		// Display tokens for debugging
		t_lex_list *temp = tokens;
		while (temp)
		{
			printf("Token: '%s', Type: %d\n", temp->s, temp->a_type);
			temp = temp->next;
		}
	}
	return 0;
}
# include "../includes/minishell.h"
//madnsachi FREE f CASE DYALL ERRORS W CTRL D KBEL MADOZ N EXEC


void free_lex_list(t_lex_list *token)
{
	while (token)
	{
		free(token->s);
		token = token->next;
	}
}


void lopo()
{
	system("leaks minishell");
}

int main(int argc, char **argv)
{
	char		*input;
	t_lex_list	*tokens;
	int status = 0;

	(void)argc;
	(void)argv;
	atexit(lopo);
	while (1)
	{
		status = 0;
		input = readline("\033[32mminishell$ \033[0m");
		if (!input)
		{
			//men baedd free_everything_then_break...
			free_lex_list(tokens);
			break;
		}	
		if (input[0])
			add_history(input);
			
		tokens = lexing_the_thing(input, &status);
		if(status != 0)
		{
			free(input);
			continue;
		}
		
		set_the_arg_type(tokens);
		
		handle_syntax_errors(tokens, &status);
		if (status != 0)
		{
			// free(input);
			// free_list(&tokens);
			continue;
		}
		
		t_lex_list *temp = tokens;
		while (temp)
		{
			printf("Token: '%s', Type: %d\n", temp->s, temp->a_type);
			temp = temp->next;
		}
		free_lex_list(tokens);
	}
	return 0;
}
# include "../includes/minishell_new.h"

int main(int argc, char **argv)
{
	char		*input;
	t_lex_list	*tokens;

	while (1)
	{
		input = readline("\033[32mminishell$ \033[0m");
		tokens = lexing_the_thing(input);
		if (!input)
			break ;
		if (input)
			add_history(tokens->s);
		while (tokens)
		{
			printf("%s\n", tokens->s);
			tokens = tokens->next;
		}
	}
}
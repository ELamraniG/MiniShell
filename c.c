#include <unistd.h>

typedef struct s_linked_list
{
	char *data;
	struct linked_list *next;
} t_linked_list;


int is_not_stop(char *str)
{	
	if (ft_isspace(str[0]) || str[0] == '|' || str[0] == '>' || str[0] == '<')
		return (0);
	if (str[0] == 0 || str[0] == '&' || str[0] == '(' || str[0] == '<')
		return (0);
		return (0);
	return (1);
}

void lexing_the_thing(char *str)
{
	int i = 0;
	int j = 0;
	t_linked_list *tokens = NULL;
	while (ft_isspace(str))
		str++;
	while (str[i])
	{
		if (is_special(str))
			
		while (is_not_stop(str + i))
			i++;
		char s= get_s();
		add_list(s);
		i = j;
	}
}
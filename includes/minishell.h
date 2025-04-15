#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef enum e_q_flags
{
	NQ,
	SQ,
	DQ
}						t_q_flags;

typedef struct s_lex_list
{
	char				*s;
	struct s_lex_list	*next;
	t_q_flags			q_type;
	int					is_space;
}						t_lex_list;
char					*ft_strdup(const char *s1);
int						ft_isspace(int c);
char					*ft_substr(char const *s, unsigned int start,
							size_t len);

size_t					ft_strlen(const char *s);
size_t					ft_strlcpy(char *dst, const char *src, size_t dstsize);

void					get_next_quote(char *str, int *i, char c);
t_lex_list				*lexing_the_thing(char *str);

void					add_to_list(t_lex_list **ll, char *str, t_q_flags quote,
							int is_space);
t_lex_list				*new_node(char *str, t_q_flags quote, int is_space);

#endif
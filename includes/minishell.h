#ifndef MINISHELL_H
#define MINISHELL_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct s_lexical_list {
	char	*s;
	struct	s_lexical_list *next;
} t_lexical_list;

#endif
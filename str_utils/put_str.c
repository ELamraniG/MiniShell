#include "../includes/minishell.h"


void ft_putstr_fd(int fd, char *str)
{
	if (!str || ft_strlen(str) == 0)
		return;
	write(fd,str,ft_strlen(str));
}
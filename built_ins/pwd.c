#include "../includes/minishell.h"

int	exec_pwd(int n)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd && n)
	{
		printf("%s\n", cwd);
		free(cwd);
		return (0); 
	}
	return (1); 
}
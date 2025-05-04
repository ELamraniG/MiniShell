#include "../includes/minishell.h"

void	pwd(int n)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd && n)
	{
		printf("%s\n", cwd);
		free(cwd);
	}
}
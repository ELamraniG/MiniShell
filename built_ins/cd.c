#include "../includes/minishell.h"

void	change_dir(char **av)
{
	char	current_dir[1000];
	char	*new_dir;
	getcwd(current_dir, sizeof(current_dir));
	int		i = ft_strlen(current_dir);
	while (current_dir[i] != '/')
		i--;
	new_dir = ft_substr(current_dir, 0, i);

	if (!ft_strcmp(av[1], "cd") && !ft_strcmp(av[2], ".."))
		chdir(new_dir);
}

int main(int ac, char **av)
{
	// char	cwd[10000];
	// getcwd(cwd, sizeof(cwd));
	// printf("%s\n", cwd);

	// chdir("/Users/jhamdaou/Desktop/minishell_git");
	// getcwd(cwd, sizeof(cwd));
	// printf("%s\n", cwd);

	// chdir("..");
	// getcwd(cwd, sizeof(cwd));
	// printf("%s\n", cwd);
	change_dir(av);



}
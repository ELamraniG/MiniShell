#include "includes/minishell.h"

static int wach_exists(const char *str, const char *pat) {
    int sLen = strlen(str);
    int pLen = strlen(pat);
    int s = 0, p = 0;
    int star = -1, match = -1;

    while (s < sLen) {
        // Match character
        if (p < pLen && pat[p] == str[s]) {
            s++;
            p++;
        }
        // Match '*'
        else if (p < pLen && pat[p] == '*') {
            star = p;
            match = s;
            p++;
        }
        // Backtrack if previous '*' exists
        else if (star != -1) {
            p = star + 1;
            match++;
            s = match;
        }
        // No match
        else {
            return 0;
        }
	}

    // Check for remaining characters in pattern
    while (p < pLen && pat[p] == '*') {
        p++;
    }
	printf("%d %d\n\n",p,pLen);
    return p == pLen;
}


static void ft_realloc(char ***args,char *s, int *size,int **is_space)
{
    int i = 0;
    int *new_is_space = malloc(sizeof(int) * (*size + 1));
    char **new_args = malloc(sizeof(char*) * (*size +2));
    int *tmpint_free;
    new_args[*size] = 0;
    while (i < *size)
    {
        new_is_space[i] = is_space[0][i];
        new_args[i] = args[0][i];
        i++;
    }
    new_args[i] = ft_strdup(s);
    new_args[*size + 1] = 0;
    (*size)++;
    char **tmp_free = *args;
    *args = new_args;
    tmpint_free = is_space[0];
    *is_space = new_is_space;
    free(tmpint_free);
    free(tmp_free);
    return ;
}

int it_has_etoil(char *str)
{
	int i = 0;
	while (str[i])
	{
		if (str[i] == '*')
			return (1);
		i++;
		}
	return (0);
}

static void handle_single_wild_card(char ***args,char *current_arg,int old_is_space,int k,int *size,int **is_space)
{
		DIR *r;
		struct dirent *reads;
	int found = 0;
	
	if (it_has_etoil(current_arg))
	{
		r = opendir(".");
		reads = readdir(r);
		while (reads)
		{
			if (wach_exists(current_arg,reads->d_name))
			{
				found = 1;
				ft_realloc(args,reads->d_name,size,is_space);
				is_space[0][*size - 1] = 1;
			}
			reads = readdir(r);
		}
		if (found == 0)
		{
			{
				ft_realloc(args,current_arg,size,is_space);
				is_space[0][*size - 1] = 1;
			}
		}
	closedir(r);
	}
	else
	{
		ft_realloc(args,current_arg,size,is_space);
		is_space[0][*size - 1] = old_is_space;
	}
}

void handle_wild_card(t_ast_tree *node)
{
    int k = 0;
	char **args = NULL;
	int size = 0;
    int *is_space = NULL;
    
    k = 0;
    while (node->args[k])
    {
        handle_single_wild_card(&args,node->args[k],node->is_space[k],k,&size,&is_space);
        k++;
    }
    k = 0;
	while (node->args[k])
		free(node->args[k++]);
    k = 0;
	while (args[k])
		printf("%s\n",args[k++]);
	// free(node->args[k]);
	free(node->args);
    node->args = args;
   
    node->arg_counter = size;
	free(node->is_space);
    node->is_space = is_space;
	free(node->q_type);
	node->q_type = malloc(sizeof(int) * size);
}




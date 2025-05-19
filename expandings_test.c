#include "includes/minishell.h"

void ft_realloc(char ***args,char *s, int *size,int **is_space)
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
    new_args[i] = s;
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


static void trim_the_args(char ***args,int size)
{
	printf("%d\n",size);
	char *tmp_free = args[0][size - 1];
	args[0][size - 1] = ft_strtrim(args[0][size - 1]," ");
	free(tmp_free);
    return ;
}

static char *get_keyy(char *str,t_env_list *env,int prev_pos,int *i,int status)
{
	(*i)++;
    char *tmp = NULL;
	// i skip the $
	int len = 0;
	while (str[*i])
	{
		if (str[*i] == '?')
        {
			(*i)++;
			return ft_strdup("?");
        }
		if (str[*i] == '$')
        {
			return ft_substr(str,prev_pos + 1,*i - prev_pos -  1);
        }
		len++;
		(*i)++;
	}
	if (len == 0)
		return ft_strdup("$");
	return (ft_strdup(str + prev_pos + 1));
}


static int has_space_at_the_end(char *s)
{
	if (!s)
		return 0;
	int strl = ft_strlen(s);
	if (strl == 0)
		return 0;
	if (s[strl - 1] == ' ')
		return 1;
	return 0;
}

static int has_space_at_the_beginning(char *s)
{
	if (!s)
		return 0;
	int strl = ft_strlen(s);
	if (strl == 0)
		return 0;
	if (s[0] == ' ')
		return 1;
	return 0;
}

static void expanded_for_single_word(char ***args,char *str,t_env_list *env, int status,int old_is_space,int k,int *size,int q_type,int **is_space)
{
	int i = 0;
    int j = 0;
    if (q_type == SQ)
    {
        ft_realloc(args,ft_strdup(str), size,is_space);
        is_space[0][*size - 1] = old_is_space;
        return ;
    }
    int flag = 0;
	int prev_pos=0;
    char **dble;
    char *tmp3;

	while(str[i])
	{
        j = 0;
		if (str[i] == '$')
		{
            flag = 1;
			char *tmp = ft_substr(str,prev_pos,i - prev_pos);
			prev_pos = i;
			if (tmp[0] != 0)
			{
				ft_realloc(args,tmp, size,is_space);
                is_space[0][*size - 1] = old_is_space;
			}
			else 
				free(tmp);
			char *tmp2 = get_keyy(str,env,prev_pos,&i,status);
			if (!ft_strcmp(tmp2,"?"))
			{
				tmp3=ft_itoa(status);
				flag = 0;
			}
			else
			{
            t_env_list *t;
            t = get_env_value(env,tmp2);
            if (!t)
			{
			    tmp3 = ft_strdup("");
				 ft_realloc(args,tmp3, size,is_space);
				 prev_pos = i;
				 continue;
			}
            else
			
                 tmp3 = ft_strdup(t->value);
			}
            free(tmp2);
            dble = ft_split_for_expand(tmp3,' ');
            free(tmp3);
            while (dble[j])
            {
                ft_realloc(args,dble[j], size,is_space);
				is_space[0][*size - 1] = 1;
				if (j == 0)
				{
					if(has_space_at_the_beginning(args[0][*size - 1]) && *size > 1)
						is_space[0][*size - 2] = 1;
				}
                j++;
				if (dble[j] == NULL)
				{
					if(has_space_at_the_end(args[0][*size - 1]))
						is_space[0][*size - 1] = 1;
					else 
						is_space[0][*size - 1] = old_is_space;
				}
				trim_the_args(args,*size);
            }	
            free(dble);
			prev_pos = i;
		}
        else 
            i++;
	}
	if (flag == 0)
	{

		char *tmp = ft_substr(str, prev_pos, ft_strlen(str) - prev_pos);
        ft_realloc(args,tmp, size,is_space);
		is_space[0][*size - 1] = old_is_space;
	}
}



void I_HATE_EXPANDING(t_ast_tree *node,t_env_list *env, int status)
{
    int k = 0;
	char **args = NULL;
	int size = 0;
    int *is_space = NULL;
    

    printf("Previous args:\n");
    while (node->args[k])
    {
        printf("[%d]: '%s'\n", k, node->args[k]);
        k++;
    }
    
    k = 0;
    while (node->args[k])
    {
        expanded_for_single_word(&args,node->args[k],env,  status,node->is_space[k],k,&size,node->q_type[k],&is_space);
        k++;
    }
    
 
    printf("Current (expanded) args:\n");
    for (int i = 0; i < size; i++)
    {
        printf("[%d]: '%s'\n", i, args[i]);
    }
    
    node->args = args;
    node->arg_counter = size; 
    node->is_space = is_space;
	int *tmp_int_free = node->q_type;
	node->q_type = malloc(sizeof(int) * size - 1);
	free(tmp_int_free);
}
















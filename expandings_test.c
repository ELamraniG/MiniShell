#include "includes/minishell.h"


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
			return ft_itoa(status);
        }
		if (str[*i] == '$')
			return ft_substr(str,prev_pos + 1,*i - prev_pos -  1);
		len++;
		(*i)++;
	}
	if (len == 0)
		return ft_strdup("$");
	return (ft_strdup(str + prev_pos + 1));
}

static int calculate_splitted_expanded_for_single_word(char *str,t_env_list *env, int status)
{
	int res = 0;
	int i = 0;
    int j = 0;
	int prev_pos=0;
    char **dble;
    char *tmp3;
	while(str[i])
	{
        j = 0;
		if (str[i] == '$')
		{
            prev_pos = i;
			char *tmp = ft_substr(str,prev_pos,i - prev_pos);
            free(tmp);
			char *tmp2 = get_keyy(str,env,prev_pos,&i,status);
            t_env_list *t;
            t = get_env_value(env,tmp2);
            if (!t)
			    tmp3 = ft_strdup(""); 
            else
                 tmp3 = ft_strdup(t->value); 
            free(tmp2);
            dble = ft_split(tmp3,' ');
            free(tmp3);
            while (dble[j])
            {
                free(dble[j]);
                j++;
                res++;
            }	
            free(dble);
		}
        else 
         i++;

	}
    if (res == 0)
        return 1;
    return res;
}

static int calculate_splitted_expanded(t_ast_tree *node,t_env_list *env, int status)
{
	int i = 0;
	int res = 0;
	while (node->args[i])
	{
        if (node->q_type[i] != SQ)
		    res += calculate_splitted_expanded_for_single_word(node->args[i],env,status);
        else 
            res++;
        i++;
	}
    return res;
}

void set_the_new_args(t_ast_tree *node,char **new_args,t_env_list *env,int status)
{
    int i = 0;
    int j = 0;
	while (node->args[i])
	{
        if (node->q_type[i] != SQ)
		    expanded_for_single_arg(node->args[i],args,env,status);
        else 
        i++;
	}
    return res;
}

void expanddd(t_ast_tree *node,t_env_list *env,int status)
{
	char **new_args= NULL;
	int malc = calculate_splitted_expanded(node,env, status);
    printf("malc = %d\n",malc);
    new_args = malloc(sizeof(char *) * (malc + 1));
    new_args[malc] = NULL;
    set_the_new_args(node,new_args,env,status);
}



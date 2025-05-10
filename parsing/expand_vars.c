#include "../includes/minishell.h"

static int is_valid_var_char(char c, int first_char)
{
    if (first_char)
        return (ft_isalpha(c) || c == '_');
    return (ft_isalnum(c) || c == '_');
}


static char *get_key(char *str, int *i)
{
    int start = *i;
    int len = 0;

    (*i)++;
    
    if (str[*i] == '?')
    {
        (*i)++;
        return ft_strdup("?");
    }
    
    while (str[*i] && is_valid_var_char(str[*i], len == 0))
    {
        (*i)++;
        len++;
    }
    if (len == 0)
        return ft_strdup("$");
    
    return ft_substr(str, start + 1, len);
}

static char *get_value(t_env_list *env, char *key, int *status)
{
    if (ft_strcmp(key, "?") == 0)
    {
        return ft_itoa(*status);
    }
    
    t_env_list *tmp = env;
    while (tmp)
    {
        if (ft_strcmp(tmp->key, key) == 0)
            return ft_strdup(tmp->value);
        tmp = tmp->next;
    }
    
    return ft_strdup("");  
}


static char *var_to_str(char *str, t_env_list *env, int *status, int quote_type)
{
    char *result = ft_strdup("");
    char *key;
    char *var_value;
    char *tmp;
    int i = 0;
    int last_pos = 0;
    
    while (str[i])
    {

        if (quote_type == SQ)
        {
            i++;
            continue;
        }
        
        if (str[i] == '$')
        {

            if (i > last_pos)
            {
                tmp = result;
                char *before_var = ft_substr(str, last_pos, i - last_pos);
                result = ft_strjoin(result, before_var);
                free(tmp);
                free(before_var);
            }
            
            int start_var = i;
            key = get_key(str, &i);
            var_value = get_value(env, key, status);
            tmp = result;
            result = ft_strjoin(result, var_value);
            free(tmp);
            free(key);
            free(var_value);
            
            last_pos = i;
        }
        else
        {
            i++;
        }
    }
    if (str[last_pos])
    {
        tmp = result;
        char *after_var = ft_substr(str, last_pos, i - last_pos);
        result = ft_strjoin(result, after_var);
        free(tmp);
        free(after_var);
    }
    
    free(str);
    return result;
}

void expand_variables(t_ast_tree *node, t_env_list *env, int *status)
{
    int i = 0;

    if (!node || !node->args)
        return;

    while (i < node->arg_counter && node->args[i])
    {
        if (node->q_type)
            node->args[i] = var_to_str(node->args[i], env, status, node->q_type[i]);
        else
            node->args[i] = var_to_str(node->args[i], env, status, NQ);
        i++;
    }
    //redirs should have q type too later do it
}

#include "../includes/minishell.h"


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
    
    if (str[*i] && (ft_isalpha(str[*i]) || str[*i] == '_'))
    {
        (*i)++;
        len++;
        
        while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
        {
            (*i)++;
            len++;
        }
    }
    else if (str[*i] == '$')
    {
        return (ft_substr(str, start + 1, len));
    }
    else
    {
        if (len == 0)
            return ft_strdup("$");
    }
    
    return ft_substr(str, start + 1, len);
}

static char *get_value(t_env_list *env, char *key, int *status)
{
    if (ft_strcmp(key, "?") == 0)
        return ft_itoa(*status);
    
    while (env)
    {
        if (ft_strcmp(env->key, key) == 0)
            return ft_strdup(env->value);
        env = env->next;
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

static char *join_expanded_parts(t_redirect *redir, t_env_list *env, int *status)
{
    char *result = ft_strdup("");
    char *tmp;
    int i = 0;
    
    while (i < redir->file_str_count)
    {
        if (redir->file_name[i])
        {
            char *expanded = var_to_str(ft_strdup(redir->file_name[i]), env, status, redir->q_types[i]);
            tmp = result;
            result = ft_strjoin(result, expanded);
            free(tmp);
            free(expanded);
        }
        i++;
    }
    
    return result;
}

char **split_expanded_args(char *expanded_str)
{
    char **result = NULL;
    int arg_count = 0;
    int i = 0, j = 0, start = 0;
    int in_word = 0;
    if (!expanded_str || expanded_str[0] == '\0') {
        result = malloc(sizeof(char *) * 2);
        result[0] = ft_strdup("");
        result[1] = NULL;
        return result;
    }
    while (expanded_str[i])
    {
        if (!ft_isspace(expanded_str[i]) && !in_word)
        {
            in_word = 1;
            arg_count++;
        }
        else if (ft_isspace(expanded_str[i]) && in_word)
            in_word = 0;
        i++;
    }
    if (arg_count == 0) {
        result = malloc(sizeof(char *) * 2);
        result[0] = ft_strdup("");
        result[1] = NULL;
        return result;
    }
    result = malloc(sizeof(char *) * (arg_count + 1));
    if (!result)
        return NULL;
    i = 0;
    in_word = 0;
    j = 0;
    while (expanded_str[i])
    {
        if (!ft_isspace(expanded_str[i]) && !in_word)
        {
            in_word = 1;
            start = i;
        }
        else if (ft_isspace(expanded_str[i]) && in_word)
        {
            result[j++] = ft_substr(expanded_str, start, i - start);
            in_word = 0;
        }
        i++;
    }
    if (in_word)
        result[j++] = ft_substr(expanded_str, start, i - start);
    result[j] = NULL;
    return result;
}

void expanddd(t_ast_tree *node, t_env_list *env, int status)
{
    int i = 0;
    char **new_args = NULL;
    int new_arg_count = 0;
    int j, k;
    if (!node->args)    
        return;
    while (i < node->arg_counter && node->args[i])
    {
        node->args[i] = var_to_str(node->args[i], env, &status, node->q_type[i]);
        i++;
    }
    i = 0;
    while (i < node->arg_counter && node->args[i])
    {
        if (node->q_type[i] == NQ && ft_strchr(node->args[i], ' ')) {
            char **tmp_split = split_expanded_args(node->args[i]);
            int splitc = 0;
            while (tmp_split && tmp_split[splitc]) splitc++;
            new_arg_count += splitc;
            if (tmp_split) {
                int z = 0;
                while (tmp_split[z]) free(tmp_split[z++]);
                free(tmp_split);
            }
        } else {
            new_arg_count++;
        }
        i++;
    }
    if (new_arg_count == node->arg_counter)
        return;  
    new_args = malloc(sizeof(char *) * (new_arg_count + 1));
    j = 0;
    for (i = 0; i < node->arg_counter; i++)
    {
        if (node->q_type[i] != NQ || !ft_strchr(node->args[i], ' '))
        {
            new_args[j++] = node->args[i];
        }
        else
        {
            char **split_args = split_expanded_args(node->args[i]);
            if (split_args)
            {
                k = 0;
                while (split_args[k])
                {
                    new_args[j++] = split_args[k++];
                }
                free(split_args); 
                free(node->args[i]);
            }
        }
    }
    new_args[j] = NULL;
    free(node->args);
    node->args = new_args;
    node->arg_counter = j;
    free(node->q_type);
    node->q_type = malloc(sizeof(int) * j);
    for (i = 0; i < j; i++)
        node->q_type[i] = NQ;
    free(node->is_space);
    node->is_space = malloc(sizeof(int) * j);
    for (i = 0; i < j; i++)
        node->is_space[i] = 1;
    if (j > 0)
        node->is_space[j-1] = 0;
}

void expand_variables(t_ast_tree *node, t_env_list *env, int *status)
{
    int i = 0;

    if (node->args) {
        while (i < node->arg_counter && node->args[i])
        {
                node->args[i] = var_to_str(node->args[i], env, status, node->q_type[i]);
            i++;
        }
    }
    
    t_redirect *redir = node->redirect;
    while (redir)
    {
        if (redir->type != HEREDOC)
        {
            char *final_file_name = join_expanded_parts(redir, env, status);
            redir->final_file_name = final_file_name;
        }
        redir = redir->next;
    }
}

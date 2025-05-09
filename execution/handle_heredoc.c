#include "../includes/minishell.h"

static void create_a_heredoc(t_ast_tree *node, t_redirect *redir, int i)
{
    int pipes[2];
    char *tmp = NULL;
    if (pipe(pipes) == -1)
    {
        perror(NULL);
        return ;
    }
    redir->heredoc = pipes[0];
    char *input = NULL;
    while(1337)
    {
        input = readline(">");
        if (!input)
        {
            ft_putstr_fd(2,"minishell: warning: here-document at line ");
            tmp = ft_itoa(i);
            ft_putstr_fd(2,tmp);
            free(tmp);
            ft_putstr_fd(2," delimited by end-of-file (wanted `");
            ft_putstr_fd(2,redir->file_name);
            ft_putstr_fd(2,"')\n");
            close(pipes[1]);
            break;
        }
        if (!ft_strcmp(redir->file_name, input))
        {
            close(pipes[1]);
            break;
        }
        tmp = input;
        input = ft_strjoin(tmp,"\n");
        free(tmp);
        write(pipes[1],input,ft_strlen(input));
        free(input);
    }
    close(pipes[1]);
    free(input);
}

void handle_heredoc(t_ast_tree *node, int i)
{
    t_redirect *redir = node->redirect;
    while(redir)
    {
        if (redir->type == HEREDOC)
            create_a_heredoc(node,redir,i);
        redir = redir->next;
    }
}
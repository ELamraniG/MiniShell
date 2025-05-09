#include "../includes/minishell.h"

static void create_a_heredoc(t_ast_tree *node, t_redirect *redir, int i)
{
    int fd = open("/tmp/lopo.txt",O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1)
    {
        printf("/tmp/lopo.txt wasnt created\n\n\n");
        perror(NULL);
        return ;
    }
    
    redir->heredoc = fd;
    unlink("/tmp/lopo.txt");
    char *input = NULL;
    while(1337)
    {
        char *input = readline(">");
        if (!input)
        {
            ft_putstr_fd(2,"minishell: warning: here-document at line ");
            char *tmp = ft_itoa(i);
            ft_putstr_fd(2,tmp);
            free(tmp);
            ft_putstr_fd(2," delimited by end-of-file (wanted `");
            ft_putstr_fd(2,redir->file_name);
            ft_putstr_fd(2,"')\n");
            break;
        }
        if (!ft_strcmp(redir->file_name, input))
            break;
        write(fd,input,ft_strlen(input));
        free(input);
        
    }
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
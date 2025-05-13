/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:                                             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 10:00:00                     #+#    #+#             */
/*   Updated: 2025/05/14 10:00:00                     ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void execute_command(t_ast_tree *node, int *status, t_env_list *env)
{
    void	(*original_sigint)(int);
    pid_t	pid;
    int		cmd_status;

    if (!node || !node->args || !node->args[0])
        return;

    // Handle built-in commands directly (no need for fork)
    if (is_builtin(node->args[0]))
    {
        *status = execute_builtin(node, env);
        return;
    }

    // Save original signal handler and ignore SIGINT in parent
    original_sigint = signal(SIGINT, SIG_IGN);
    
    pid = fork();
    if (pid == -1)
    {
        ft_putstr_fd(2, "minishell: fork: Resource temporarily unavailable\n");
        return;
    }
    
    if (pid == 0)
    {
        // Child process
        signal(SIGINT, handle_sig_exec_ve);
        signal(SIGQUIT, SIG_DFL);
        
        // Execute command
        execute_external_command(node, env);
        exit(*status);
    }
    else
    {
        // Parent process
        waitpid(pid, &cmd_status, 0);
        
        // Restore original signal handler
        signal(SIGINT, original_sigint);
        
        // Update status based on child's exit status
        if (WIFEXITED(cmd_status))
            *status = WEXITSTATUS(cmd_status);
        else if (WIFSIGNALED(cmd_status))
            *status = 128 + WTERMSIG(cmd_status);
    }
}

// Add or update your existing command execution functions to use this approach

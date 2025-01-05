/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 15:32:53 by jadyar            #+#    #+#             */
/*   Updated: 2025/01/05 18:40:48 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* static int	execute_pipeline(t_command *current, t_shell *shell,
		struct sigaction *sa_old_int, struct sigaction *sa_old_quit)
{
	int		prev_pipe[2];
	pid_t	last_pid;

	prev_pipe[0] = -1;
	prev_pipe[1] = -1;
	last_pid = 0;
	while (current)
	{
		if (!setup_pipeline_steps(current, prev_pipe, &last_pid, shell))
		{
			g_exit_status = 1;
			cleanup_pipeline_resources(prev_pipe, NULL);
			return (g_exit_status);
		}
		current = current->next;
	}
	wait_for_children(last_pid);
	sigaction(SIGINT, sa_old_int, NULL);
	sigaction(SIGQUIT, sa_old_quit, NULL);
	return (g_exit_status);
} */

/* void wait_for_pipeline(t_shell *shell)
{
    int     status;
    int     first_error = 0;
    int     last_status = 0;
    pid_t   waited_pid;
    int     i;

    for (i = 0; i < shell->pid_count; i++)
    {
        waited_pid = waitpid(shell->pids[i], &status, 0);
        if (waited_pid == -1)
        {
            perror("waitpid failed");
            if (!first_error)
                first_error = 1;
            continue;
        }
        
        if (WIFEXITED(status))
        {
            last_status = WEXITSTATUS(status);
            // Keep track of the first error we see
            if (last_status != 0 && !first_error)
                first_error = last_status;
        }
        else if (WIFSIGNALED(status))
        {
            last_status = 128 + WTERMSIG(status);
            if (!first_error)
                first_error = last_status;
                
            // Handle signals appropriately
            if (WTERMSIG(status) == SIGINT)
                write(STDERR_FILENO, "\n", 1);
            else if (WTERMSIG(status) == SIGQUIT)
                write(STDERR_FILENO, "Quit (core dumped)\n", 19);
        }
    }
    
    // For pipelines, we want to preserve the exit status of the last command
    // unless it succeeded and we saw an earlier error
    if (shell->pid_count > 1)
    {
        if (last_status == 0 && first_error)
            g_exit_status = first_error;
        else
            g_exit_status = last_status;
    }
    else
        g_exit_status = first_error ? first_error : last_status;
} */

void wait_for_pipeline(t_shell *shell)
{
    int     status;
    int     last_status = 0;
    pid_t   waited_pid;
    int     i;

    // Wait for all processes
    for (i = 0; i < shell->pid_count; i++)
    {
        waited_pid = waitpid(shell->pids[i], &status, 0);
        if (waited_pid == -1)
        {
            perror("waitpid failed");
            g_exit_status = 1;
            return;
        }

        // Handle normal process exit
        if (WIFEXITED(status))
            last_status = WEXITSTATUS(status);
        // Handle process terminated by signal
        else if (WIFSIGNALED(status))
        {
            last_status = 128 + WTERMSIG(status);
            if (WTERMSIG(status) == SIGINT)
                write(STDERR_FILENO, "\n", 1);
            else if (WTERMSIG(status) == SIGQUIT)
                write(STDERR_FILENO, "Quit (core dumped)\n", 19);
        }
    }

    g_exit_status = last_status;
}

static int execute_pipeline(t_command *current, t_shell *shell,
        struct sigaction *sa_old_int, struct sigaction *sa_old_quit)
{
    int     prev_pipe[2];
    int     cmd_count;
    
    // Count commands in pipeline
    cmd_count = 0;
    t_command *temp = current;
    while (temp)
    {
        cmd_count++;
        temp = temp->next;
    }
    
    // Allocate PID array
    shell->pids = malloc(sizeof(pid_t) * cmd_count);
    if (!shell->pids)
        return (cleanup_and_exit(ERR_MALLOC, NULL, 1, shell));
    shell->pid_count = 0;
    
    prev_pipe[0] = -1;
    prev_pipe[1] = -1;

    // Execute pipeline
    while (current)
    {
        if (!setup_pipeline_steps(current, prev_pipe, shell))
        {
            g_exit_status = 1;
            cleanup_pipeline_resources(prev_pipe, NULL);
            free(shell->pids);
            shell->pids = NULL;
            return (g_exit_status);
        }
        current = current->next;
    }

    // Wait for all processes
    wait_for_pipeline(shell);
    
    free(shell->pids);
    shell->pids = NULL;
    
    sigaction(SIGINT, sa_old_int, NULL);
    sigaction(SIGQUIT, sa_old_quit, NULL);
    return (g_exit_status);
}


int	execute_commands(t_shell *shell)
{
	t_command			*current;
	struct sigaction	sa_old_int;
	struct sigaction	sa_old_quit;

	current = shell->cmnd_lst;
	if (current && !current->is_valid)
	{
		print_command_not_found(current);
		g_exit_status = 127;
		return (g_exit_status);
	}
	setup_execution_signals(&sa_old_int, &sa_old_quit);
	if (current && !current->next && current->args
		&& is_builtin(current->args[0]))
	{
		sigaction(SIGINT, &sa_old_int, NULL);
		sigaction(SIGQUIT, &sa_old_quit, NULL);
		return (execute_single_builtin(current, shell));
	}
	return (execute_pipeline(current, shell, &sa_old_int, &sa_old_quit));
}

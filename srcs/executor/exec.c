/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 15:32:53 by jadyar            #+#    #+#             */
/*   Updated: 2025/01/15 13:34:37 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int restore_fd_flags(int fd, int flags)
{
    int current_flags = fcntl(fd, F_GETFL);
    fprintf(stderr, "DEBUG: Restoring fd %d flags:\n", fd);
    fprintf(stderr, "DEBUG: - Current: %d (0x%x)\n", current_flags, current_flags);
    fprintf(stderr, "DEBUG: - Target:  %d (0x%x)\n", flags, flags);

    // Clear only standard flags
    int clear_mask = ~(O_APPEND | O_ASYNC | O_NONBLOCK);
    current_flags &= clear_mask;

    // Then set the desired flags
    int new_flags = (current_flags | (flags & ~clear_mask));
    
    if (fcntl(fd, F_SETFL, new_flags) == -1) {
        fprintf(stderr, "DEBUG: Failed to set flags: %s\n", strerror(errno));
        return -1;
    }

    int result = fcntl(fd, F_GETFL);
    fprintf(stderr, "DEBUG: - Result:  %d (0x%x)\n", result, result);
    return result;
}

static int execute_pipeline(t_command *current, t_shell *shell,
        struct sigaction *sa_old_int, struct sigaction *sa_old_quit)
{
    int     prev_pipe[2];
    pid_t   last_pid;
    int     original_stdin_flags;
	int     original_stdin_fd;

    prev_pipe[0] = -1;
    prev_pipe[1] = -1;
    last_pid = 0;

    // Save original stdin state
    original_stdin_fd = dup(STDIN_FILENO);
    original_stdin_flags = fcntl(STDIN_FILENO, F_GETFL);
    fprintf(stderr, "DEBUG: Parent process initial state:\n");
    fprintf(stderr, "DEBUG: - STDIN backup fd: %d\n", original_stdin_fd);
    fprintf(stderr, "DEBUG: - STDIN original flags: %d (0x%x)\n", 
            original_stdin_flags, original_stdin_flags);
    
    while (current)
    {
        if (!setup_pipeline_steps(current, prev_pipe, &last_pid, shell))
        {
            fprintf(stderr, "DEBUG: Pipeline step setup failed\n");
            g_exit_status = 1;
        }
        current = current->next;
    }

    // After pipeline completion, restore stdin to original state
    fprintf(stderr, "DEBUG: Restoring parent process stdin state\n");
    if (dup2(original_stdin_fd, STDIN_FILENO) == -1) {
        fprintf(stderr, "DEBUG: Failed to restore STDIN: %s\n", strerror(errno));
    }
    restore_fd_flags(STDIN_FILENO, original_stdin_flags);
    if (fcntl(STDIN_FILENO, F_SETFL, original_stdin_flags) == -1) {
        fprintf(stderr, "DEBUG: Failed to restore STDIN flags: %s\n", strerror(errno));
    }
    close(original_stdin_fd);

    int final_flags = fcntl(STDIN_FILENO, F_GETFL);
    fprintf(stderr, "DEBUG: Final STDIN flags: %d (0x%x)\n", 
            final_flags, final_flags);
    restore_exec_signals(sa_old_int, sa_old_quit);

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
	setup_exec_signals(&sa_old_int, &sa_old_quit);
	if (current && !current->next && current->args
		&& is_builtin(current->args[0]))
	{
		sigaction(SIGINT, &sa_old_int, NULL);
		sigaction(SIGQUIT, &sa_old_quit, NULL);
		return (execute_single_builtin(current, shell));
	}
	return (execute_pipeline(current, shell, &sa_old_int, &sa_old_quit));
}

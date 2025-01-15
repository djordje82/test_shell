/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_manager.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 14:43:39 by dodordev          #+#    #+#             */
/*   Updated: 2025/01/15 13:55:46 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* int	setup_child_pipe_io(int in_fd, int out_fd)
{
	int	stdin_flags;
	int	stdout_flags;

	fprintf(stderr, "DEBUG: Setting up pipe IO - in_fd: %d, out_fd: %d\n",
		in_fd, out_fd);
	fprintf(stderr, "DEBUG: Current STDIN_FILENO: %d, STDOUT_FILENO: %d\n",
			STDIN_FILENO, STDOUT_FILENO);
	if (in_fd != -1)
	{
		if (dup2(in_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 failed in_fd");
			return (0);
		}
		fprintf(stderr, "DEBUG: Duped in_fd to STDIN\n");
		close(in_fd);
	}
	if (out_fd != -1)
	{
		if (dup2(out_fd, STDOUT_FILENO) == -1)
		{
			perror("dup2 failed out_fd");
			return (0);
		}
		close(out_fd);
	}
	return (1);
} */

// Changed from setup_pipe_io to setup_child_pipe_io to be more explicit
// First, let's fix setup_child_pipe_io (formerly setup_pipe_io)
int setup_child_pipe_io(int input_fd, int output_fd)
{
    fprintf(stderr, "DEBUG: Child pipe IO - in_fd: %d, out_fd: %d\n", 
            input_fd, output_fd);

    // Save original flags
    int original_stdin_flags = fcntl(STDIN_FILENO, F_GETFL);
    int original_stdout_flags = fcntl(STDOUT_FILENO, F_GETFL);
    
    fprintf(stderr, "DEBUG: Original flags - stdin: 0x%x, stdout: 0x%x\n",
            original_stdin_flags, original_stdout_flags);

    if (input_fd != -1)
    {
        if (dup2(input_fd, STDIN_FILENO) == -1)
        {
            perror("dup2 failed in_fd");
            return (0);
        }
        // Restore stdin flags after dup2
        if (fcntl(STDIN_FILENO, F_SETFL, original_stdin_flags) == -1) {
            fprintf(stderr, "DEBUG: Failed to restore stdin flags\n");
        }
    }

    if (output_fd != -1)
    {
        if (dup2(output_fd, STDOUT_FILENO) == -1)
        {
            perror("dup2 failed out_fd");
            return (0);
        }
        // Restore stdout flags after dup2
        if (fcntl(STDOUT_FILENO, F_SETFL, original_stdout_flags) == -1) {
            fprintf(stderr, "DEBUG: Failed to restore stdout flags\n");
        }
    }

    fprintf(stderr, "DEBUG: Final child flags - stdin: 0x%x, stdout: 0x%x\n",
            fcntl(STDIN_FILENO, F_GETFL), fcntl(STDOUT_FILENO, F_GETFL));

    return (1);
}

void	handle_parent_process(int *prev_pipe, int *pipe_fd)
{
	fprintf(stderr, "DEBUG: Parent process (PID: %d) handling pipes\n",
		getpid());
	if (prev_pipe)
	{
		fprintf(stderr, "DEBUG: Closing prev_pipe fds: [%d,%d]\n", prev_pipe[0],
			prev_pipe[1]);
		if (prev_pipe[0] != -1)
			close(prev_pipe[0]);
		if (prev_pipe[1] != -1)
			close(prev_pipe[1]);
		prev_pipe[0] = -1;
		prev_pipe[1] = -1;
	}
	if (pipe_fd && prev_pipe)
	{
		prev_pipe[0] = pipe_fd[0];
		prev_pipe[1] = pipe_fd[1];
	}
}

static int	setup_pipeline_io(int input_fd, int output_fd, int *prev_pipe,
		int *pipe_fd)
{
	fprintf(stderr, "DEBUG: Pipeline IO setup starting\n");
	if (!setup_child_pipe_io(input_fd, output_fd))
	{
		handle_pipe_io_error(prev_pipe, pipe_fd);
		return (0);
	}
	if (input_fd != -1)
		close(input_fd);
	if (output_fd != -1)
		close(output_fd);
	if (prev_pipe)
	{
		close_pipe_ends(prev_pipe);
	}
	if (pipe_fd)
	{
		if (pipe_fd[0] != -1 && pipe_fd[0] != input_fd)
			close(pipe_fd[0]);
		if (pipe_fd[1] != -1 && pipe_fd[1] != output_fd)
			close(pipe_fd[1]);
	}
	fprintf(stderr, "DEBUG: Pipeline IO setup complete\n");
	return (1);
}

void	handle_pipeline_child(t_command *cmd, int *prev_pipe, int *pipe_fd,
		t_shell *shell)
{
	int	status;
	int	input_fd;
	int	output_fd;
	int	original_stdin_flags;
	int	stdin_backup;
	int	stdout_backup;
	int	pre_exec_flags;
	int	final_flags;

	fprintf(stderr, "DEBUG: Pipeline child start (PID: %d, PPID: %d)\n",
		getpid(), getppid());
	// Save original stdin state BEFORE any pipe operations
	original_stdin_flags = fcntl(STDIN_FILENO, F_GETFL);
	fprintf(stderr, "DEBUG: Child initial STDIN flags: %d (0x%x)\n",
		original_stdin_flags, original_stdin_flags);
	// Save backup FDs after saving flags
	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	fprintf(stderr, "DEBUG: Child backup FDs - stdin: %d, stdout: %d\n",
		stdin_backup, stdout_backup);
	input_fd = get_input_fd(prev_pipe);
	output_fd = get_output_fd(pipe_fd);
	// Setup pipe IO with flag preservation
	if (!setup_pipeline_io(input_fd, output_fd, prev_pipe, pipe_fd))
	{
		fprintf(stderr, "DEBUG: Pipeline IO setup failed\n");
		if (fcntl(STDIN_FILENO, F_SETFL, original_stdin_flags) == -1)
		{
			fprintf(stderr, "DEBUG: Failed to restore child STDIN flags: %s\n",
				strerror(errno));
		}
		close(stdin_backup);
		close(stdout_backup);
		handle_pipe_io_error(prev_pipe, pipe_fd);
		exit(1);
	}
	// After pipe setup, restore flags
	fprintf(stderr, "DEBUG: Restoring child STDIN flags after pipe setup\n");
	if (fcntl(STDIN_FILENO, F_SETFL, original_stdin_flags) == -1)
	{
		fprintf(stderr, "DEBUG: Failed to restore child STDIN flags: %s\n",
			strerror(errno));
	}
	if (!setup_redirections(cmd))
	{
		fprintf(stderr, "DEBUG: Redirection setup failed\n");
		if (fcntl(STDIN_FILENO, F_SETFL, original_stdin_flags) == -1)
		{
			fprintf(stderr, "DEBUG: Failed to restore child STDIN flags: %s\n",
				strerror(errno));
		}
		close(stdin_backup);
		close(stdout_backup);
		close_pipe_ends(pipe_fd);
		exit(1);
	}
	setup_child_signal();
	// Check flags before command execution
	pre_exec_flags = fcntl(STDIN_FILENO, F_GETFL);
	fprintf(stderr, "DEBUG: Child STDIN flags before exec: %d (0x%x)\n",
		pre_exec_flags, pre_exec_flags);
	status = execute_single_command(cmd, shell);
	fprintf(stderr, "DEBUG: Child command executed with status: %d\n", status);
	// Final flag check before exit
	final_flags = fcntl(STDIN_FILENO, F_GETFL);
	fprintf(stderr, "DEBUG: Child final STDIN flags: %d (0x%x)\n", final_flags,
		final_flags);
	close(stdin_backup);
	close(stdout_backup);
	close_pipe_ends(pipe_fd);
	exit(status);
}

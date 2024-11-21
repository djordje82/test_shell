/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:04:48 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/21 17:13:21 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*This function initializes a pipeline. It sets up pipe file descriptors and checks for parent-only builtins.*/
static int	init_pipeline(t_command *current, int *pipe_fd, t_shell *shell)
{
	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	if (current->next && pipe(pipe_fd) == -1) //check what pipe() does
		return (cleanup_and_exit("pipe failed", NULL, 1, shell));
	if (current->args && is_parent_only_builtin(current->args[0]))
		handle_builtin_cmd(current, shell);
	return (1);
}

void close_pipe(int *prev_pipe, int *pipe_fd)
{
    if (prev_pipe)
    {
        if (prev_pipe[0] != -1)
            close(prev_pipe[0]);
        if (prev_pipe[1] != -1)
            close(prev_pipe[1]);
        prev_pipe[0] = prev_pipe[1] = -1;
    }
    if (pipe_fd)
    {
        if (pipe_fd[0] != -1)
            close(pipe_fd[0]);
        if (pipe_fd[1] != -1)
            close(pipe_fd[1]);
        pipe_fd[0] = pipe_fd[1] = -1;
    }
}

/* static void	close_pipe(int *prev_pipe, int *pipe_fd)
{
	if (prev_pipe[0] != -1)
		close(prev_pipe[0]);
	if (pipe_fd[1] != -1)
		close(pipe_fd[1]);
} */

/*This function sets up pipeline steps. It initializes pipe file descriptors, checks for parent-only builtins, and prepares for process creation.*/
/* int	setup_pipeline_steps(t_command *current, int *prev_pipe, pid_t *last_pid,
		t_shell *shell)
{
	int pipe_fd[2];
	pid_t pid;
	int	status;

	if (!init_pipeline(current, pipe_fd, shell))
		return (0);
	if (!create_process(&pid, shell))
		return (0);
	if (pid == 0)
		handle_pipeline_child(current, prev_pipe, pipe_fd, shell);
	if (!current->is_valid)
	{
		close_pipe(prev_pipe, pipe_fd);
		print_command_not_found(current);
		exit(127);
	}
	if (!current->prev)
	{
		waitpid(pid, &status, WNOHANG);
		if (WIFEXITED(status) && !WEXITSTATUS(status))
		{
			close_pipe(prev_pipe, pipe_fd);
			return (0);
		}
	}
	if (!current->next)
		*last_pid = pid;
	handle_parent_process(prev_pipe, pipe_fd);
	return (1);
} */
int setup_pipeline_steps(t_command *current, int *prev_pipe, pid_t *last_pid,
        t_shell *shell)
{
    int pipe_fd[2];
    pid_t pid;

    // Validate command before creating pipe or forking
    if (!current->is_valid)
    {
        if (!current->prev)// If it's first command
        {
            print_command_not_found(current);
            return (0);  // Stop entire pipeline
        }
        // If not first command, continue pipeline
    }
    //signal(SIGINT, SIG_IGN);
    // Initialize pipeline only if command is valid or not first
    if (!init_pipeline(current, pipe_fd, shell))
        return (0);

    if (!create_process(&pid, shell))
    {
        close_pipe(prev_pipe, pipe_fd);
        return (0);
    }

    if (pid == 0)  // Child process
    {
        //signal(SIGINT, SIG_DFL);
        if (!current->is_valid)
        {
            close_pipe(prev_pipe, pipe_fd);
            exit(127);  // Exit child with error
        }
        handle_pipeline_child(current, prev_pipe, pipe_fd, shell);
    }

    // Parent process
    if (!current->next)
        *last_pid = pid;
    handle_parent_process(prev_pipe, pipe_fd);
    return (1);
}

/*This function sets up pipe redirections. It duplicates file descriptors to standard input and output, and closes the pipe file descriptors.*/
void setup_pipe_redirections(int *prev_pipe, int *pipe_fd)
{
    // Current order might cause issues with pipe handling
    // Better order:
    if (pipe_fd[1] != -1)
    {
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[0]);
        close(pipe_fd[1]);
    }
    if (prev_pipe[0] != -1)
    {
        dup2(prev_pipe[0], STDIN_FILENO);
        close(prev_pipe[0]);
        close(prev_pipe[1]);
    }
}

/*This function sets up and executes a command in a pipeline. It handles signals, redirections, and execution errors.*/
void	execute_pipeline_cmd(t_command *cmd, char *cmd_path, t_shell *shell)
{
	setup_child_signal();
	if (!setup_redirections(cmd))
		exit(1);
	execve(cmd_path, cmd->args, shell->envp);
	handle_command_errors(cmd_path, cmd->args[0]);
}
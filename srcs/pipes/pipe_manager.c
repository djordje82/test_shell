#include "minishell.h"

// Creates a new pipe and handles errors
int create_pipe(int pipe_fd[2], t_shell *shell)
{
    pipe_fd[0] = -1;
    pipe_fd[1] = -1;
    
    if (pipe(pipe_fd) == -1)
        return (cleanup_and_exit("pipe failed", NULL, 1, shell));
    return (1);
}

// Safely closes pipe file descriptors
void close_pipe_ends(int pipe_fd[2])
{
    if (!pipe_fd)
        return;
    if (pipe_fd[0] != -1)
    {
        close(pipe_fd[0]);
        pipe_fd[0] = -1;
    }
    if (pipe_fd[1] != -1)
    {
        close(pipe_fd[1]);
        pipe_fd[1] = -1;
    }
}

// Sets up pipe redirections for input and output
int setup_pipe_io(int in_fd, int out_fd)
{
    if (in_fd != -1)
    {
        if (dup2(in_fd, STDIN_FILENO) == -1)
            return (0);
        close(in_fd);
    }
    if (out_fd != -1)
    {
        if (dup2(out_fd, STDOUT_FILENO) == -1)
            return (0);
        close(out_fd);
    }
    return (1);
}

/*This function restores the standard file descriptors to their original values.*/
void	restore_std_fds(int stdin_fd, int stdout_fd)
{
	if (dup2(stdin_fd, STDIN_FILENO) == -1)
		perror("dup2 stdin");
	if (dup2(stdout_fd, STDOUT_FILENO) == -1)
		perror("dup2 stdout");
	close(stdin_fd);
	close(stdout_fd);
}

void	handle_pipeline_child(t_command *cmd, int *prev_pipe, int *pipe_fd, t_shell *shell)
{
	int	status;

	if (!setup_pipe_io(
        prev_pipe ? prev_pipe[0] : -1,  // Input from previous pipe
        pipe_fd ? pipe_fd[1] : -1       // Output to current pipe
    ))
        exit(1);

    // Close remaining pipe ends we don't need
    close_pipe_ends(prev_pipe);
    if (pipe_fd)
        close(pipe_fd[0]);  // Only close read end of current pipe

    if (!setup_redirections(cmd))
        exit(1);
    setup_child_signal();
    status = execute_single_command(cmd, shell);
    exit(status);
}

void handle_parent_process(int *prev_pipe, int *pipe_fd)
{
	close_pipe_ends(prev_pipe);  // Close previous pipe ends
	
	if (pipe_fd && pipe_fd[1] != -1)  // If there's a next command
	{
		prev_pipe[0] = pipe_fd[0];
		prev_pipe[1] = pipe_fd[1];
	}
	else
		close_pipe_ends(pipe_fd);
}
#include "minishell.h"

static int execute_single_command(t_command *cmd, t_shell *shell)
{
    int stdin_fd;
    int stdout_fd;
    int status;

    //printf("Debug: Executing single command\n");
    //printf("Debug: Command args:\n");
    int i = 0;
    while (cmd->args && cmd->args[i])
    {
        //printf("Debug: args[%d] = '%s'\n", i, cmd->args[i]);
        i++;
    }

    stdin_fd = dup(STDIN_FILENO);
    stdout_fd = dup(STDOUT_FILENO);
    if (!setup_redirections(cmd))
        return (0);
    
    //printf("Debug: Checking if builtin: %s\n", cmd->args[0]);
    if (is_builtin(cmd->args[0]))
    {
        //printf("Debug: Executing builtin command\n");
        status = execute_builtin(cmd, shell);
        
        // Only exit if it's the exit command AND the status indicates we should exit
        if (ft_strncmp(cmd->args[0], "exit", 5) == 0 && status != 1)
        {
            reset_redirections(stdin_fd, stdout_fd);
            exit(status);
        }
    }
    else
    {
        //printf("Debug: Executing external command\n");
        status = execute_external(cmd, shell);
    }
    
    reset_redirections(stdin_fd, stdout_fd);
    return (status);
}


/*static int	execute_pipeline(t_command *cmd, t_shell *shell)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		status;

	if (pipe(pipe_fd) == -1)
		return (exit_error(ERR_PIPE, NULL, 1, shell));
	pid = fork();
	if (pid == -1)
		return (exit_error(ERR_FORK, NULL, 1, shell));
	if (pid == 0)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
		status = execute_single_command(cmd, shell);
		exit(status);
	}
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	return (pid);
}*/

/*static void	wait_for_children(pid_t last_pid)
{
	int	status;

	waitpid(last_pid, &status, 0);
	while (wait(NULL) > 0)
		;
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
}*/

int execute_commands(t_shell *shell)
{
    t_command *current = shell->cmnd_lst;
    int prev_pipe[2] = {-1, -1};
    int status = 0;
    struct sigaction sa_old_int;
    struct sigaction sa_old_quit;

    // Save old signal handlers 
    sigaction(SIGINT, NULL, &sa_old_int);
    sigaction(SIGQUIT, NULL, &sa_old_quit);

    // Set parent process signal handling during command execution
    struct sigaction sa;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = SIG_IGN;  // Parent ignores signals during command execution
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    // If it's a single builtin command, execute it in the parent process
    if (current && !current->next && current->args && is_builtin(current->args[0]))
    {
        int stdin_fd = dup(STDIN_FILENO);
        int stdout_fd = dup(STDOUT_FILENO);
        
        if (!setup_redirections(current))
            return (0);
        
        status = execute_builtin(current, shell);
        reset_redirections(stdin_fd, stdout_fd);
        
        // Only exit if it was an exit command and status indicates we should exit
        if (ft_strncmp(current->args[0], "exit", 5) == 0 && status != 1)
        {
            shell->running = false;
            shell->exit_status = status;
        }
        return status;
    }

    // For pipelines or external commands, use the existing fork logic
    while (current)
    {
        int pipe_fd[2] = {-1, -1};
        
        if (current->next && pipe(pipe_fd) == -1)
            return (exit_error("pipe failed", NULL, 1, shell));

        pid_t pid = fork();
        if (pid == -1)
            return (exit_error("fork failed", NULL, 1, shell));

        if (pid == 0)
        {
            // Child process
            if (prev_pipe[0] != -1)
            {
                dup2(prev_pipe[0], STDIN_FILENO);
                close(prev_pipe[0]);
                close(prev_pipe[1]);
            }
            if (pipe_fd[1] != -1)
            {
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[0]);
                close(pipe_fd[1]);
            }
            status = execute_single_command(current, shell);
            exit(status);
        }

        // Parent process
        if (prev_pipe[0] != -1)
        {
            close(prev_pipe[0]);
            close(prev_pipe[1]);
        }
        
        if (pipe_fd[1] != -1)
        {
            prev_pipe[0] = pipe_fd[0];
            prev_pipe[1] = pipe_fd[1];
        }

        current = current->next;
    }

    // Wait for all child processes
    if (wait(&status) > 0)
    {
        if (WIFSIGNALED(status))
        {
            g_exit_status = 128 + WTERMSIG(status);
            if (WTERMSIG(status) == SIGQUIT)
                write(STDERR_FILENO, "Quit (core dumped)\n", 18);
            else if (WTERMSIG(status) == SIGINT)
                write(STDERR_FILENO, "\n", 1);
        }
        else
            g_exit_status = WEXITSTATUS(status);
    }

    // Restore original signal handlers
    sigaction(SIGINT, &sa_old_int, NULL);
    sigaction(SIGQUIT, &sa_old_quit, NULL);

    return WEXITSTATUS(status);
}
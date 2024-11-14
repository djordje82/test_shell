#include "minishell.h"

int is_parent_only_builtin(char *cmd)
{
    return (ft_strncmp(cmd, "cd", 3) == 0 ||
            ft_strncmp(cmd, "export", 7) == 0 ||
            ft_strncmp(cmd, "unset", 6) == 0);
}

static void setup_signal_handlers(struct sigaction *sa_old_int, 
    struct sigaction *sa_old_quit)
{
    // Save old handlers
    sigaction(SIGINT, NULL, sa_old_int);
    sigaction(SIGQUIT, NULL, sa_old_quit);
    
    // Set parent process to ignore signals during command execution
    struct sigaction sa;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = SIG_IGN;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
}

static int handle_single_builtin(t_command *cmd, t_shell *shell)
{
    int status;
    int stdin_fd;
    int stdout_fd;

    stdin_fd = dup(STDIN_FILENO);
    stdout_fd = dup(STDOUT_FILENO);
    if (!setup_redirections(cmd))
        return (0);
    status = execute_builtin(cmd, shell);
    reset_redirections(stdin_fd, stdout_fd);
    if (ft_strncmp(cmd->args[0], "exit", 5) == 0 && status != 1)
    {
        shell->running = false;
        shell->exit_status = status;
    }
    return (status);
}

static void handle_child_process(t_command *cmd, int *prev_pipe, 
    int *pipe_fd, t_shell *shell)
{
    int status;
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
    handle_signals_child();  // Use your existing function
    status = execute_single_command(cmd, shell);
    exit(status);  // Always exit with the command's status
}

static void handle_parent_process(int *prev_pipe, int *pipe_fd)
{
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
}

/*static void handle_wait_status(int status, pid_t wpid, pid_t last_pid)
{
    if (WIFSIGNALED(status))
    {
        g_exit_status = 128 + WTERMSIG(status);
        if (WTERMSIG(status) == SIGQUIT)
            write(STDERR_FILENO, "Quit (core dumped)\n", 18);
        else if (WTERMSIG(status) == SIGINT)
            write(STDERR_FILENO, "\n", 1);
    }
    else if (WIFEXITED(status))
    {
        int exit_code = WEXITSTATUS(status);
        if (wpid == last_pid)
            g_exit_status = exit_code;
    }
}*/

static void wait_for_children(pid_t last_pid)
{
    int     status;
    pid_t   wpid;

    while ((wpid = wait(&status)) > 0)
    {
        if (WIFSIGNALED(status))
        {
            g_exit_status = 128 + WTERMSIG(status);
            if (WTERMSIG(status) == SIGQUIT)
                write(STDERR_FILENO, "Quit (core dumped)\n", 18);
            else if (WTERMSIG(status) == SIGINT)
                write(STDERR_FILENO, "\n", 1);
        }
        else if (WIFEXITED(status))
        {
            if (wpid == last_pid)
                g_exit_status = WEXITSTATUS(status);
        }
    }
}

int handle_pipeline(t_command *current, int *prev_pipe, 
    pid_t *last_pid, t_shell *shell)
{
    int     pipe_fd[2];
    pid_t   pid;

    pipe_fd[0] = -1;
    pipe_fd[1] = -1;
    if (current->next && pipe(pipe_fd) == -1)
        return (exit_error("pipe failed", NULL, 1, shell));
    if (current->args && is_parent_only_builtin(current->args[0]))
        execute_builtin(current, shell);
    pid = fork();
    if (pid == -1)
        return (exit_error("fork failed", NULL, 1, shell));
    if (pid == 0)
        handle_child_process(current, prev_pipe, pipe_fd, shell);
    if (!current->next)
        *last_pid = pid;
    handle_parent_process(prev_pipe, pipe_fd);
    return (1);
}

int execute_single_command(t_command *cmd, t_shell *shell)
{
    int stdin_fd;
    int stdout_fd;
    int status;

    // Skip if no arguments at all
    if (!cmd->args)
        return (0);

    // If first arg is empty but there are more args, shift them left
    if (cmd->args[0] && !cmd->args[0][0])
    {
        int i = 0;
        while (cmd->args[i + 1])
        {
            cmd->args[i] = cmd->args[i + 1];
            i++;
        }
        cmd->args[i] = NULL;
    }

    // Now check if we have any arguments left
    if (!cmd->args[0])
        return (0);

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

int execute_commands(t_shell *shell)
{
    t_command *current;
    int prev_pipe[2];
    struct sigaction sa_old_int;
    struct sigaction sa_old_quit;
    pid_t last_pid;

    current = shell->cmnd_lst;
    prev_pipe[0] = -1;
    prev_pipe[1] = -1;
    last_pid = 0;
    setup_signal_handlers(&sa_old_int, &sa_old_quit);
    if (current && !current->next && current->args 
        && is_builtin(current->args[0]))
        return (handle_single_builtin(current, shell));
    while (current)
    {
        if (!handle_pipeline(current, prev_pipe, &last_pid, shell))
            return (1);
        current = current->next;
    }
    wait_for_children(last_pid);
    sigaction(SIGINT, &sa_old_int, NULL);
    sigaction(SIGQUIT, &sa_old_quit, NULL);
    return (g_exit_status);
}

/*int execute_commands(t_shell *shell)
{
    t_command *current = shell->cmnd_lst;
    int prev_pipe[2] = {-1, -1};
    int status = 0;
    struct sigaction sa_old_int;
    struct sigaction sa_old_quit;
    pid_t last_pid = 0;  // Track the last process in pipeline

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

        // For export/unset/cd in a pipeline, execute in parent before forking
        if (current->args && (ft_strncmp(current->args[0], "export", 7) == 0 ||
                            ft_strncmp(current->args[0], "unset", 6) == 0 ||
                            ft_strncmp(current->args[0], "cd", 3) == 0))
        {
            execute_builtin(current, shell);
        }

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
            // Don't re-execute export/unset/cd in child
            if (current->args && !(ft_strncmp(current->args[0], "export", 7) == 0 ||
                                 ft_strncmp(current->args[0], "unset", 6) == 0 ||
                                 ft_strncmp(current->args[0], "cd", 3) == 0))
            {
                status = execute_single_command(current, shell);
            }
            exit(status);
        }

        // Track last pid for exit status
        if (!current->next)
            last_pid = pid;

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
    int last_status = 0;
    pid_t wpid;
    while ((wpid = wait(&status)) > 0)
    {
        if (WIFSIGNALED(status)) // Returns true if the child was terminated by a signal
        {
            g_exit_status = 128 + WTERMSIG(status); // Returns the exit status of the child (only valid if WIFEXITED is true)
            if (WTERMSIG(status) == SIGQUIT)
                write(STDERR_FILENO, "Quit (core dumped)\n", 18);
            else if (WTERMSIG(status) == SIGINT)
                write(STDERR_FILENO, "\n", 1);
        }
        else if (WIFEXITED(status)) // Returns true if the child terminated normally
        {
            last_status = WEXITSTATUS(status); // Returns the exit status of the child (only valid if WIFEXITED is true)
            if (wpid == last_pid)  // If this was the last process
                g_exit_status = last_status;  // Use its exit status
        }
    }

    // Restore original signal handlers
    sigaction(SIGINT, &sa_old_int, NULL);
    sigaction(SIGQUIT, &sa_old_quit, NULL);

    //return WEXITSTATUS(status);
    return g_exit_status;
}*/
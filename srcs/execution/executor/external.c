#include "minishell.h"

char **merge_args(char **orig_args, int pos, char **expanded)
{
    int orig_len = count_args(orig_args);
    int exp_len = count_args(expanded);
    int new_len = orig_len + exp_len - 1; // -1 because we're replacing one arg
    char **new_args;
    int i, j, k;

    new_args = malloc(sizeof(char *) * (new_len + 1));
    if (!new_args)
        return NULL;

    // Copy args before wildcard
    i = 0;
    while (i < pos)
    {
        new_args[i] = ft_strdup(orig_args[i]);
        i++;
    }

    // Copy expanded wildcards
    j = 0;
    while (expanded[j])
    {
        new_args[i] = ft_strdup(expanded[j]);
        j++;
        i++;
    }

    // Copy remaining args
    k = pos + 1;
    while (orig_args[k])
    {
        new_args[i] = ft_strdup(orig_args[k]);
        k++;
        i++;
    }

    new_args[i] = NULL;
    return new_args;
}

int execute_external(t_command *cmd, t_shell *shell)
{
    char    *cmd_path;
    char    **expanded_args;
    pid_t   pid;
    int     status;
    int     i;

    // Handle wildcard expansion for each argument except the command itself
    i = 1;
    while (cmd->args[i])
    {
        if (ft_strchr(cmd->args[i], '*'))
        {
            expanded_args = expand_wildcards(cmd->args[i]);
            if (expanded_args)
            {
                // Replace the wildcard argument with its expansions
                char **new_args = merge_args(cmd->args, i, expanded_args);
                if (!new_args)
                {
                    free_array((void **)expanded_args, -1);
                    return (exit_error(ERR_MEM, NULL, 1, shell));
                }
                free_array((void **)cmd->args, -1);
                cmd->args = new_args;
                i += count_args(expanded_args) - 1; // Skip the expanded arguments
                free_array((void **)expanded_args, -1);
            }
        }
        i++;
    }

    // Skip empty commands
    if (!cmd->args || !cmd->args[0] || !cmd->args[0][0])
        return (0);

    cmd_path = find_command_path(cmd->args[0], shell);
     if (!cmd_path)
    {
        // Only print error for the first command in a pipeline
        if (!cmd->prev && cmd->args[0][0])
        {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(cmd->args[0], STDERR_FILENO);
            
            // If the command starts with ./ or / or ../, treat as file path
            if (ft_strchr(cmd->args[0], '/'))
            {
                ft_putendl_fd(": No such file or directory", STDERR_FILENO);
            }
            else
            {
                ft_putendl_fd(": command not found", STDERR_FILENO);
            }
        }
        return (127);
    }

    // If we're in a pipeline (being called from handle_child_process)
    if (cmd->prev || cmd->next)
    {
        if (!setup_redirections(cmd))
            exit(1);
        execve(cmd_path, cmd->args, shell->envp);
        
        // Check if it's a directory first
        struct stat path_stat;
        stat(cmd_path, &path_stat);
        if (S_ISDIR(path_stat.st_mode))
        {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(cmd->args[0], STDERR_FILENO);
            ft_putendl_fd(": Is a directory", STDERR_FILENO);
            free(cmd_path);
            exit(126);
        }
        // Then check permissions
        if (access(cmd_path, F_OK) == 0)
        {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(cmd->args[0], STDERR_FILENO);
            ft_putendl_fd(": Permission denied", STDERR_FILENO);
            free(cmd_path);
            exit(126);
        }
        free(cmd_path);
        exit(127);
    }

    pid = fork();
    if (pid == -1)
    {
        free(cmd_path);
        return (exit_error(ERR_FORK, NULL, 1, shell));
    }
        
    if (pid == 0)
    {
        handle_signals_child();
        if (!setup_redirections(cmd))
            exit(1);
        execve(cmd_path, cmd->args, shell->envp);
        
        // Check if it's a directory first
        struct stat path_stat;
        stat(cmd_path, &path_stat);
        if (S_ISDIR(path_stat.st_mode))
        {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(cmd->args[0], STDERR_FILENO);
            ft_putendl_fd(": Is a directory", STDERR_FILENO);
            exit(126);
        }
        // Then check permissions
        if (access(cmd_path, F_OK) == 0)
        {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(cmd->args[0], STDERR_FILENO);
            ft_putendl_fd(": Permission denied", STDERR_FILENO);
            exit(126);
        }
        exit(127);
    }
    
    // Parent process
    free(cmd_path);
    waitpid(pid, &status, 0);
    
    // Handle signal-related exit status
    if (WIFSIGNALED(status))
    {
        g_exit_status = 128 + WTERMSIG(status);
        if (WTERMSIG(status) == SIGQUIT)
            write(STDERR_FILENO, "Quit (core dumped)\n", 18);
        else if (WTERMSIG(status) == SIGINT)
            write(STDERR_FILENO, "\n", 1);
        return g_exit_status;
    }
    
    return (WEXITSTATUS(status));
}

/*int execute_external(t_command *cmd, t_shell *shell)
{
    char    *cmd_path;
    pid_t   pid;
    int     status;

    cmd_path = find_command_path(cmd->args[0], shell);
    if (!cmd_path)
        return (exit_error(ERR_CMD, cmd->args[0], 127, shell));
        
    pid = fork();
    if (pid == -1)
        return (exit_error(ERR_FORK, NULL, 1, shell));
        
    if (pid == 0)
    {
        handle_signals_child();  // Reset signal handlers for child
        if (!setup_redirections(cmd))
            exit(1);
        execve(cmd_path, cmd->args, shell->envp);
        exit_error(ERR_CMD, cmd->args[0], 127, shell);
        exit(127);
    }
    
    // Parent process
    free(cmd_path);
    waitpid(pid, &status, 0);
    
    // Handle signal-related exit status
    if (WIFSIGNALED(status))
    {
        g_exit_status = 128 + WTERMSIG(status);
        if (WTERMSIG(status) == SIGQUIT)
            write(STDERR_FILENO, "Quit (core dumped)\n", 18);
        else if (WTERMSIG(status) == SIGINT)
            write(STDERR_FILENO, "\n", 1);
        return g_exit_status;
    }
    
    return (WEXITSTATUS(status));
}*/
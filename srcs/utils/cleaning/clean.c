#include "minishell.h"

void	close_files(t_node *node)
{
	int	i;

	if (node->fd_in)
	{
		i = 0;
		while (node->fd_in[i])
			ft_close(node->fd_in[i++]);
		free(node->fd_in);
	}
	if (node->fd_out)
	{
		i = 0;
		while (node->fd_out[i])
			ft_close(node->fd_out[i++]);
		free(node->fd_out);
	}
}

void	free_files(t_node *node)
{
	if (node->infile)
	{
		free_array((void **)node->infile, node->n_input);
		node->infile = NULL;
	}
	if (node->outfile)
	{
		free_array((void **)node->outfile, node->n_output);
		node->outfile = NULL;
	}
	if (node->heredoc)
	{
		free_array((void **)node->heredoc, node->n_input);
		node->heredoc = NULL;
	}
	if (node->append)
		free(node->append);
	close_files(node);
}

void	clean_node(t_node *node)
{
	free_files(node);
	if (node->command)
		free(node->command);
	if (node->path)
		free(node->path);
	if (node->args)
	{
		free_array((void **)node->args, -1);
		node->args = NULL;
	}
	if (node->hd_pipe)
	{
		free_array((void **)node->hd_pipe, node->n_input);
		node->hd_pipe = NULL;
	}
	if (node)
		free(node);
}

void	cleanup_shell(t_shell *shell)
{
	//printf("Debug: Starting shell cleanup\n");
	if (!shell)
		return;

	// Clean command list first
	if (shell->cmnd_lst)
	{
		//printf("Debug: Cleaning command list\n");
		free_cmd_list(shell->cmnd_lst);
		shell->cmnd_lst = NULL;
	}

	// Clean tokens
	if (shell->tokens)
	{
		//printf("Debug: Cleaning tokens\n");
		free_tokens(shell->tokens);
		shell->tokens = NULL;
	}

	// Clean pipes
	if (shell->pipe)
	{
		//printf("Debug: Cleaning pipes\n");
		free_array((void **)shell->pipe, shell->n_cmnds);
		shell->pipe = NULL;
	}

	// Clean PIDs
	if (shell->pid)
	{
		//printf("Debug: Cleaning PIDs\n");
		free(shell->pid);
		shell->pid = NULL;
	}

	// Environment variables should be the last to be cleaned
	// and might not need to be freed depending on how they were allocated
	if (shell->envp)
	{
		//printf("Debug: Setting environment to NULL\n");
		//shell->envp = NULL;  // Don't free envp as it might be the original environment
		char **tmp = shell->envp;
        while (*tmp)
        {
            free(*tmp);
            tmp++;
        }
        free(shell->envp);
        shell->envp = NULL;
	}

	//printf("Debug: Cleaning readline history\n");
	rl_clear_history();
	
	//printf("Debug: Shell cleanup complete\n");
}

/*void	free_list(t_command **cmnd_list)
{
	t_list	*temp;

	if (!cmnd_list || !*cmnd_list)
		return ;
	while (*cmnd_list)
	{
		temp = (*cmnd_list)->next;
		clean_node((*cmnd_list)->content);
		free(*cmnd_list);
		*cmnd_list = temp;
	}
}*/

void    free_cmd_list(t_command *cmd)
{
    t_command *temp;

    while (cmd)
    {
        temp = cmd->next;
        if (cmd->args)
            free_array((void **)cmd->args, -1);
        if (cmd->infile)
            free(cmd->infile);
        if (cmd->outfile)
            free(cmd->outfile);
        free(cmd);
        cmd = temp;
    }
}
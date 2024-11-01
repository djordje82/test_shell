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
	if (shell)
	{
		free_list(&shell->cmnd_lst);
		free_meta(shell);
		free_envp(shell);
		free_tokens(shell->tokens);
	}
	rl_clear_history();
}

void	free_list(t_list **cmnd_list)
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
}
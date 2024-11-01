#include "minishell.h"

void	ft_close(int fd)
{
	if (fd >= 0)
	{
		close(fd);
		fd = -1;
	}
}

void	free_array(void **arr, int size)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	if (size == -1)
	{
		while (arr[i])
		{
			free(arr[i]);
			i++;
		}
	}
	else
	{
		while (i < size)
		{
			free(arr[i]);
			i++;
		}
	}
	free(arr);
}

void	free_shell(t_shell *shell)
{
	if (shell->pid)
	{
		free(shell->pid);
		shell->pid = NULL;
	}
	if (shell->pipe)
	{
		free_array((void **)shell->pipe, shell->n_cmnds + 1);
		shell->pipe = NULL;
	}
}

void	free_envp(t_shell *shell)
{
	if (shell->envp)
		free_array((void **)shell->envp, -1);
	shell->envp = NULL;
}

void	free_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
}
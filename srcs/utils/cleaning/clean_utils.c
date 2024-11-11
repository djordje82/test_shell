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

	//printf("Debug: free_array called with size %d\n", size);
	if (!arr)
	{
		//printf("Debug: arr is NULL, returning\n");
		return;
	}
	
	i = 0;
	if (size == -1)
	{
		while (arr[i])
		{
			//printf("Debug: Freeing arr[%d]: '%s'\n", i, (char *)arr[i]);
			free(arr[i]);
			i++;
		}
	}
	else
	{
		while (i < size)
		{
			//printf("Debug: Freeing arr[%d]\n", i);
			free(arr[i]);
			i++;
		}
	}
	//printf("Debug: Freeing arr itself\n");
	free(arr);
	//printf("Debug: free_array complete\n");
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
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
}
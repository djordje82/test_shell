#include "minishell.h"

int	exit_error(char *err_msg, char *src, int err_code, t_shell *shell)
{
	char	*result;

	if (src)
	{
		result = ft_strjoin(src, err_msg);
		if (!result)
		{
			ft_putendl_fd(ERR_MEM, STDERR_FILENO);
			exit(-1);
		}
		ft_putendl_fd(result, STDERR_FILENO);
		free(result);
	}
	else if (err_msg)
		ft_putendl_fd(err_msg, STDERR_FILENO);
	if (shell)
	{
		if (shell->cmnd_lst)
			free_cmd_list(shell->cmnd_lst);
		if (shell->envp)
			free_array((void **)shell->envp, -1);
		if (shell->tokens)
			free_tokens(shell->tokens);
		if (shell->pipe)
			free_array((void **)shell->pipe, shell->n_cmnds);
		shell->exit_status = err_code;
	}
	rl_clear_history();
	exit(err_code);
}
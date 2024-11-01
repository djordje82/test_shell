#include "minishell.h"

int	exit_error(char *err_msg, char *src, int err_code, t_shell *meta)
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
	if (meta)
	{
		free_list(meta->cmnd_lst);
		free_meta(meta);
		free_envp(meta);
		meta->exit_code = err_code;
	}
	rl_clear_history();
	exit(err_code);
}
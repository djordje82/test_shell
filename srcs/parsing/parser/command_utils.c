#include "minishell.h"

t_command	*create_cmd_node(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->in_type = 0;
	cmd->out_type = 0;
	cmd->next = NULL;
	return (cmd);
}

void	add_cmd_node(t_command **cmd_list, t_command *new_cmd)
{
	t_command	*current;

	if (!*cmd_list)
	{
		*cmd_list = new_cmd;
		return ;
	}
	current = *cmd_list;
	while (current->next)
		current = current->next;
	current->next = new_cmd;
}

/*void	free_cmd_list(t_command *cmd)
{
	t_command	*temp;
	int		i;

	while (cmd)
	{
		temp = cmd->next;
		if (cmd->args)
		{
			i = 0;
			while (cmd->args[i])
				free(cmd->args[i++]);
			free(cmd->args);
		}
		if (cmd->infile)
			free(cmd->infile);
		if (cmd->outfile)
			free(cmd->outfile);
		free(cmd);
		cmd = temp;
	}
}*/
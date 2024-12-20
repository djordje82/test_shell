/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:00:54 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/04 16:56:01 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**insert_arg_array(char **orig_args, int pos, char **to_insert)
{
	char	**new_args;
	int		orig_count;
	int		insert_count;
	int		i;
	int		j;

	orig_count = ft_count_args(orig_args);
	insert_count = ft_count_args(to_insert);
	new_args = malloc(sizeof(char *) * (orig_count + insert_count + 1));
	if (!new_args)
		return (NULL);
	i = 0;
	while (i < pos)
		new_args[i] = ft_strdup(orig_args[i]);
	j = 0;
	while (to_insert[j])
		new_args[i++] = ft_strdup(to_insert[j++]);
	while (orig_args[pos + 1])
		new_args[i++] = ft_strdup(orig_args[pos++ + 1]);
	new_args[i] = NULL;
	return (new_args);
}

int	update_command_args(t_command *cmd, char **expanded, int pos,
		t_shell *shell)
{
	char	**new_args;

	if (!cmd || !expanded || !shell || !cmd->args)
		return (cleanup_and_exit(ERR_INVALID, NULL, 1, shell));
	new_args = insert_arg_array(cmd->args, pos, expanded);
	if (!new_args)
	{
		ft_free_array((void **)expanded, -1);
		return (cleanup_and_exit(ERR_MEM, NULL, 1, shell));
	}
	ft_free_array((void **)cmd->args, -1);
	cmd->args = new_args;
	return (ft_count_args(expanded));
}

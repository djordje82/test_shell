/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:00:54 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/15 18:00:56 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**merge_args(char **orig_args, int pos, char **expanded)
{
	char	**new_args;
	int		i;
	int		j;
	int		k;

	new_args = malloc(sizeof(char *) * (ft_count_args(orig_args)
				+ ft_count_args(expanded)));
	if (!new_args)
		return (NULL);
	i = -1;
	while (++i < pos)
		new_args[i] = ft_strdup(orig_args[i]);
	j = 0;
	while (expanded[j])
		new_args[i++] = ft_strdup(expanded[j++]);
	k = pos + 1;
	while (orig_args[k])
		new_args[i++] = ft_strdup(orig_args[k++]);
	new_args[i] = NULL;
	return (new_args);
}

int	process_expanded_args(t_command *cmd, char **expanded, int i,
		t_shell *shell)
{
	char	**new_args;

	new_args = merge_args(cmd->args, i, expanded);
	if (!new_args)
	{
		ft_free_array((void **)expanded, -1);
		return (exit_error(ERR_MEM, NULL, 1, shell));
	}
	ft_free_array((void **)cmd->args, -1);
	cmd->args = new_args;
	return (ft_count_args(expanded));
}

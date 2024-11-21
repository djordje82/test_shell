/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_cmd_node.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 19:07:08 by dodordev          #+#    #+#             */
/*   Updated: 2024/11/17 19:08:17 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*This function closes and frees the file descriptors arrays associated with a command node.*/
void	cleanup_fd_arrays(t_node *node)
{
	int	i;

	if (!node)
		return ;
	if (node->fd_in)
	{
		i = 0;
		while (node->fd_in[i])
			ft_close_fd(node->fd_in[i++]);
		free(node->fd_in);
	}
	if (node->fd_out)
	{
		i = 0;
		while (node->fd_out[i])
			ft_close_fd(node->fd_out[i++]);
		free(node->fd_out);
		node->fd_out = NULL;
	}
}

/*This function frees the file resources associated with a command node.*/
void	cleanup_file_resources(t_node *node)
{
	if (!node)
		return ;
	if (node->infile)
	{
		ft_free_array((void **)node->infile, node->n_input);
		node->infile = NULL;
	}
	if (node->outfile)
	{
		ft_free_array((void **)node->outfile, node->n_output);
		node->outfile = NULL;
	}
	if (node->heredoc)
	{
		ft_free_array((void **)node->heredoc, node->n_input);
		node->heredoc = NULL;
	}
	if (node->append)
		free(node->append);
	cleanup_fd_arrays(node);
}

/*This function frees all resources associated with a command node structure.*/
void	cleanup_command_node(t_node *node)
{
	if (!node)
		return ;
	cleanup_file_resources(node);
	if (node->command)
		free(node->command);
	if (node->path)
		free(node->path);
	if (node->args)
	{
		ft_free_array((void **)node->args, -1);
		node->args = NULL;
	}
	/* if (node->hd_pipe)
	{
		ft_free_array((void **)node->hd_pipe, node->n_input);
		node->hd_pipe = NULL;
	} */
	free(node);
}
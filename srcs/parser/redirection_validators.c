/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_validators.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 19:40:52 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/17 19:41:11 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_parent_dir_permissions(const char *filename)
{
	char	*parent_dir;
	char	*last_slash;

	parent_dir = ft_strdup(filename);
	if (!parent_dir)
		return (0);
	last_slash = ft_strrchr(parent_dir, '/');
	if (last_slash)
	{
		*last_slash = '\0';
		if (access(parent_dir, W_OK) == -1)
		{
			print_file_error(parent_dir, "Permission denied");
			free(parent_dir);
			return (0);
		}
	}
	free(parent_dir);
	return (1);
}

int	validate_output_permissions(const char *filename)
{
	struct stat	path_stat;

	if (!filename)
		return (0);
	if (access(filename, F_OK) == 0)
	{
		// File exists - check if it's a directory
		if (stat(filename, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
		{
			print_file_error(filename, "Is a directory");
			return (0);
		}
		// Check write permission
		if (access(filename, W_OK) == -1)
		{
			print_file_error(filename, "Permission denied");
			return (0);
		}
	}
	else
	{
		// File doesn't exist - check parent directory permissions
		if (!check_parent_dir_permissions(filename))
			return (0);
	}
	return (1);
}

int	validate_input_permissions(const char *filename)
{
	struct stat	path_stat;

	if (!filename)
		return (0);
	// For input files, they must exist (except for heredoc)
	if (access(filename, F_OK) == -1)
	{
		print_file_error(filename, "No such file or directory");
		return (0);
	}
	// Check if it's a directory
	if (stat(filename, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
	{
		print_file_error(filename, "Is a directory");
		return (0);
	}
	// Check read permission
	if (access(filename, R_OK) == -1)
	{
		print_file_error(filename, "Permission denied");
		return (0);
	}
	return (1);
}

// Update the existing validate_redirection function
int	validate_redirection(t_token_type type, const char *filename)
{
	// Skip validation for heredoc
	if (type == TOKEN_HEREDOC)
		return (1);

	// Validate based on redirection type
	if (type == TOKEN_REDIR_IN)
		return (validate_input_permissions(filename));
	else if (type == TOKEN_REDIR_OUT || type == TOKEN_APPEND)
		return (validate_output_permissions(filename));

	return (0);
}
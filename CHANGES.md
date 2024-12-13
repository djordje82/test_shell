List of changes:

In minishell.h

t-command struct was updated to better track file descriptors

			typedef struct s_command
			{
				char						**args;
				char						*infile;
				char						*outfile;
				int							in_type;
				int							out_type;
				int							curr_fd; //this was added to track file descriptors;
				bool						is_valid;
				struct s_command			*next;
				struct s_command			*prev;
			}								t_command;



To fix issues with heredoc following files have been updated:

In srcs/executor following has been updated:

heredoc.c
setup_redirection_utils.c
setup_redirection.c

In srcs/parser following has been updated:

command_utils.c

parse_redirections.c
	changed: int	parse_redirections(t_token **token, t_command *cmd)
				- has been updated to checks output files immediately during parsing, which should match bash's behavior of reporting output file errors before input file errors.
	added:   static int check_output_file(char *filename, t_token_type type)

In srcs/utils/cleanup
cleanup_shell_utils.c
	updated: void	cleanup_cmd_list(t_command *cmd)
			this following lines were added to properly clean file descriptors
					if (cmd->curr_fd > 2)
			close(cmd->curr_fd);
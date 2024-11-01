/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_shell.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 11:45:14 by dodordev          #+#    #+#             */
/*   Updated: 2024/10/25 13:37:12 by dodordev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_SHELL_H
# define MINI_SHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include "../libft/inc/libft.h"
# include <fcntl.h>
# include <errno.h>
# include <termios.h>

# define SHELL_NAME "minishell"
# define TOKEN_DELIMITERS " \t\n\r\a"
# define REDIR_DELIMITERS ">>"
# define PIPE_DELIMITERS "|"
# define PROMPT "minishell> "
# define SPECIAL "<>|;"

/*ERROR CODES*/
# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1
# define EXIT_ERROR 2
# define EXIT_NOT_FOUND 127
# define EXIT_NOT_EXECUTABLE 126
# define CTRL_D_EXIT_CODE 0
# define CTRL_C_EXIT_CODE 130
# define CTRL_BSLASH_EXIT_CODE 131
# define EXIT_PIPE 133
# define EXIT_FORK 134
# define EXIT_REDIRECTION 135
# define EXIT_SYNTAX 136
# define EXIT_CD 137
# define EXIT_ENV 138
# define EXIT_MALLOC 139
# define EXIT_SIGNAL 140
# define EXIT_UNEXPECTED 141
# define EXIT_NOT_IMPLEMENTED 142

/*ERROR MESSAGES*/
# define ERR_MEM "minishell: memory allocation error"
# define ERR_SYNTAX "minishell: syntax error"
# define ERR_PIPE "minishell: pipe error"
# define ERR_FORK "minishell: fork error"
# define ERR_QUOTE "minishell: unclosed quote"
# define ERR_TOKEN "minishell: syntax error near unexpe
# define ERR_SYNTAX_PIPE "minishell: syntax error near unexpected token `|'"
# define ERR_SYNTAX_NEWLINE "minishell: syntax error near unexpected token `newline'"
# define ERR_SYNTAX_REDIR "minishell: syntax error near unexpected token `redirection'"
# define ERR_CMD "minishell: command not found"
# define ERR_PERM "minishell: permission denied"

/*GLOBAL VARIABLE*/
extern int	g_exit_status;

/*STRUCTS*/
typedef struct s_node
{
	char	*command;
	char	*path;
	char	**args;
	char	**infile;
	char	**outfile;
	char	**heredoc;
	int		*append;
	int		**hd_pipe;
	int		*fd_in;
	int		*fd_out;
	int		n_input;
	int		n_output;
}	t_node;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,	// <
	TOKEN_REDIRECT_OUT,	// >
	TOKEN_APPEND,		// >>
	TOKEN_HEREDOC,		// <<
	TOKEN_SPACE,
	TOKEN_QUOTE,		// '
	TOKEN_DQUOTE,		// "
	TOKEN_ENV			// $
}	t_token_type;

typedef struct s_token
{
	char			*value;
	int				type;
	struct s_token	*next;
}	t_token;

typedef struct s_command
{
	char		**args;
	char		*infile;
	char		*outfile;
	int			in_type;
	int			out_type;
	struct s_command	*next;
}			t_command;

typedef struct s_shell
{
	char	**envp;
	t_list	*cmnd_lst;
	t_token	*tokens;
	int		**pipe;
	pid_t	*pid;
	int		n_cmnds;
	int		exit_status;
	bool	running;
}			t_shell;

/*INITIALIZATION*/
void			init_shell(t_shell *shell, char **envp);


/*TOKENIZER*/
t_token			*tokenize_input(char *input, t_shell *shell);
t_token			*create_token(char *value, t_token_type type);
void			add_token(t_token **tokens, t_token *new_token);
t_token_type	get_token_type(char c);
char			*extract_word(char *input, int *i);
char			*extract_quoted(char *input, int *i, char quote_type);
t_token			*handle_operator(char *input, int *i);
int				handle_quotes(char *input, int *i, char quote_type);

/*PARSER*/
t_command		*parse_command(t_token **token);
int				parse_tokens(t_shell *shell);
t_token			*tokenize_input(char *input, t_shell *shell);

/*PARSER /UTILS*/
t_command		*create_cmd_node(void);
void			add_cmd_node(t_command **cmd_list, t_command *new_cmd);
void			free_cmd_list(t_command *cmd);
int				parse_arguments(t_token **token, t_command *cmd);
int				parse_redirections(t_token **token, t_command *cmd);

/*BUILTINS*/
int				ft_cd(char **args, t_shell *shell);
int				ft_pwd(void);
void			ft_echo(char **args);
void			ft_export(char **args);
void			ft_unset(char **args);
void			ft_env(void);
void			ft_exit(char **args);

/*ERROR HANDLING*/
int				exit_error(char *err_msg, char *src, int err_code, t_shell *shell);
int				minishell_clean(t_shell *shell, int exit_code);
void			minishell_reset(t_shell *shell);
void			print_syntax_error(const char *msg);

/*CLEANING*/
void			free_shell(t_shell *shell);
void			free_envp(t_shell *shell);
void			free_tokens(t_token *tokens);
void			close_files(t_node *node);
void			free_files(t_node *node);
void			free_list(t_list **cmnd_list);
void			clean_node(t_node *node);
void			free_array(void **arr, int size);
void			ft_close(int fd);
void			cleanup_shell(t_shell *shell);

/*UTILS*/
int				count_args(char **args);
int				is_numeric(const char *str);
int				is_whitespace(char c);

/*UTILS /ERRORS*/
int				exit_error(char *err_msg, char *src, int err_code, t_shell *shell);	

/*UTILS /SIGNALS*/
void			setup_signals(void);
void			handle_signals_child(void);
void			handle_eof(t_shell *shell);
void			disable_ctrl_chars(void);
void			signal_handler(int signum);
void			signal_handler_child(int signum);

#endif

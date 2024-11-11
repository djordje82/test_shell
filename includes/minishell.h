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
# include <stdbool.h>
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
# include <limits.h>

# define SHELL_NAME "minishell"
//# define TOKEN_DELIMITERS " \t\n\r\a"
//# define REDIR_DELIMITERS ">>"
//# define PIPE_DELIMITERS "|"
# define PROMPT "minishell> "
//# define SPECIAL "<>|;"
//# define OPERATORS "><"

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
# define ERR_TOKEN "minishell: syntax error near unexpected token"
# define ERR_SYNTAX_PIPE "minishell: syntax error near unexpected token `|'"
# define ERR_SYNTAX_NEWLINE "minishell: syntax error near unexpected token `newline'"
# define ERR_SYNTAX_REDIR "minishell: syntax error near unexpected token `redirection'"
# define ERR_CMD "minishell: command not found"
# define ERR_PERM "minishell: permission denied"

/*GLOBAL VARIABLE*/
extern int	g_exit_status;

typedef enum e_char_type
{
    CHAR_NORMAL,     // Regular characters
    CHAR_WHITESPACE, // Space, tab, newline
    CHAR_PIPE,       // |
    CHAR_REDIR_IN,   // <
    CHAR_REDIR_OUT,  // >
    CHAR_QUOTE,      // '
    CHAR_DQUOTE,     // "
    CHAR_SEMICOLON,  // ; (for future implementation)
    CHAR_ESCAPE      // \ (for future implementation)
} t_char_type;

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
	t_command	*cmnd_lst;
	t_token		*tokens;
	int			**pipe;
	pid_t		*pid;
	int			n_cmnds;
	int			exit_status;
	bool		running;
}			t_shell;

/*MAIN*/
void			init_shell_struct(t_shell *shell, char **envp);
int				init_minishell(t_shell *shell, char **env, int argc, char **argv);
void			shell_loop(t_shell *shell);
void			handle_input_line(char *input, t_shell *shell);
void			cleanup_command_data(t_shell *shell);


/*TOKENIZER*/
char			*get_var_name(const char *str);
char			*ft_strjoin_free(char *s1, char *s2);
t_token *create_token(char *value, t_token_type type);  // Swap parameters order
t_token	*tokenize_input(const char *input, t_shell *shell);
t_token	*handle_word(const char *input, int *i, t_shell *shell);
t_token *handle_pipe(const char *input, int *pos);
t_token	*handle_quote(char *input, int *i, t_shell *shell);
t_token *get_next_token(const char *input, int *pos, t_shell *shell);
void			add_token(t_token **tokens, t_token *new_token);
t_token_type	get_token_type(char c);
char			*extract_word(const char *input, int *pos);
char			*extract_quoted(char *input, int *pos, char quote_type);
t_token *handle_operator(const char *input, int *i);
int				handle_quotes(char *input, int *i, char quote_type);
char			*expand_env_vars(char *str, t_shell *shell);

/*PARSER*/
t_command		*parse_command(t_token **token);
int				parse_tokens(t_shell *shell);

/*PARSER /UTILS*/
t_command		*create_cmd_node(void);
void			add_cmd_node(t_command **cmd_list, t_command *new_cmd);
void			free_cmd_list(t_command *cmd);
int				parse_arguments(t_token **token, t_command *cmd);
int				parse_redirections(t_token **token, t_command *cmd);

/*EXECUTOR*/
int				execute_commands(t_shell *shell);

/*BUILTINS*/
int				ft_cd(char **args, t_shell *shell);
int				ft_pwd(char **args, t_shell *shell);
int				ft_echo(char **args, t_shell *shell);
int				ft_export(char **args, t_shell *shell);
int				ft_unset(char **args, t_shell *shell);
int				ft_env(char **args, t_shell *shell);
int				ft_exit(char **args, t_shell *shell);

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
//void			free_list(t_list **cmnd_list);
void			clean_node(t_node *node);
void			free_array(void **arr, int size);
void			ft_close(int fd);
void			cleanup_shell(t_shell *shell);
void			free_cmd_list(t_command *cmd);

/*UTILS*/
t_char_type		get_char_type(char c);
int				is_special_char(char c);
int				count_args(char **args);
int				is_numeric(const char *str);
int				is_whitespace(char c);
//int				is_metacharacter(char c);
int				is_operator(char c);
int				is_quote(char c);
int				is_valid_identifier(char *name);

/*UTILS /ERRORS*/
int				exit_error(char *err_msg, char *src, int err_code, t_shell *shell);	

/*UTILS /SIGNALS*/
void			setup_signals(void);
void			handle_signals_child(void);
void			handle_eof(t_shell *shell);
void			disable_ctrl_chars(void);
void			signal_handler(int signum);
void			signal_handler_child(int signum);

/*ENVIRONMENT*/
char			*get_env_value(char *name, t_shell *shell);
int				update_env_value(char *name, char *value, t_shell *shell);

/*ENVIRONMENT /UTILS*/
int				count_envp(char **envp);

/*ENVIRONMENT /ERRORS*/
int				exit_error(char *err_msg, char *src, int err_code, t_shell *shell);

/*ENVIRONMENT /UTILS*/
char			*get_home_dir(t_shell *shell);
int				update_pwd_vars(t_shell *shell);
int				find_env_index(char *name, char **env);
char			*create_env_string(char *name, char *value);
int				has_equals_sign(char *str);
int				check_env_args(char **args);
void			print_env_var(char *env_var);

/*BUILTINS /UTILS*/
int				is_n_flag(char *arg);
void			print_args(char **args, int start, int n_flag);

/*ERRORS*/
int				exit_error(char *err_msg, char *src, int err_code, t_shell *shell);
int				syntax_error(char *err_msg, char *src);

/*BUILTINS /EXECUTION*/
int				execute_builtin(t_command *cmd, t_shell *shell);

/*BUILTINS /CD*/
char			*get_home_dir(t_shell *shell);
char			*expand_path(char *path, t_shell *shell);
int				update_pwd_vars(t_shell *shell);
char			*expand_path(char *path, t_shell *shell);

/*BUILTINS /EXIT*/
int				is_numeric_arg(char *str);
long long		ft_atoll(const char *str);
int				count_args(char **args);

/*BUILTINS /EXPORT*/
int				is_valid_identifier(char *name);
void			print_export_error(char *arg);
//char			*get_var_name(char *arg);
char			*get_var_value(char *arg);
int				update_existing_var(char *arg, t_shell *shell);
int				add_new_var(char *arg, t_shell *shell);
char			**copy_env(char **envp);
void			print_exported_var(char *var);

/*BUILTINS /UNSET*/
int				is_valid_identifier(char *name);
void			print_identifier_error(char *arg);
int				remove_env_var(char *name, t_shell *shell);

/*EXECUTOR /REDIRECTIONS*/
int				setup_redirections(t_command *cmd);
void			reset_redirections(int stdin_fd, int stdout_fd);


/*EXECUTOR /UTILS*/
int				is_builtin(char *cmd);
int				execute_builtin(t_command *cmd, t_shell *shell);
char			*get_cmd_path(char *cmd, char **paths);
char			*find_command_path(char *cmd, t_shell *shell);


/*EXECUTOR /ERRORS*/
int				exit_error(char *err_msg, char *src, int err_code, t_shell *shell);


/*EXECUTOR /EXEC*/
int				execute_command(t_command *cmd, t_shell *shell);

/*EXECUTOR /EXTERNAL*/
int				execute_external(t_command *cmd, t_shell *shell);

#endif

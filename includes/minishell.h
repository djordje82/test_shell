/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 11:45:14 by dodordev          #+#    #+#             */
/*   Updated: 2024/12/09 12:39:23 by jadyar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/inc/libft.h"
# include <dirent.h> //for wildcard expansion opendir(), readdir(), closedir()
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

# define SHELL_NAME "minishell"
# define PROMPT "minishell> "

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

/*ERROR CODES*/
# define EXIT_SUCCESS 0
# define EXIT_ERROR_NUMERIC 2
# define EXIT_ERROR_ARGS 1
# define EXIT_FAILURE 1
# define EXIT_ERROR 2
# define CMD_NOT_FOUND 127
# define CMD_NOT_EXECUTABLE 126
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
//# define EXIT_UNEXPECTED 141
# define EXIT_NOT_IMPLEMENTED 142

/*ERROR MESSAGES*/
# define ERR_MEM "minishell: memory allocation error"
# define ERR_INVALID "minishell: invalid argument"
# define ERR_SYNTAX "minishell: syntax error"
# define ERR_PIPE "minishell: pipe error"
# define ERR_FORK "minishell: fork error"
# define ERR_QUOTE "minishell: unclosed quote"
# define ERR_TOKEN "minishell: syntax error near unexpected token"
# define ERR_SYNTAX_PIPE "minishell: syntax error near unexpected token `|'"
# define ERR_CMD "minishell: command not found"
# define ERR_BATCH "minishell: arguments not allowed  extra!!"

# define FILE_PERMS 0644
# define REDIR_TRUNC 1
# define REDIR_APPEND 2
# define REDIR_INPUT 3
# define REDIR_OUTPUT 4
# define REDIR_HEREDOC 5

/*GLOBAL VARIABLE*/

extern volatile sig_atomic_t	g_exit_status;

typedef enum e_char_type
{
	CHAR_WHITESPACE,
	CHAR_PIPE,
	CHAR_REDIR_IN,
	CHAR_REDIR_OUT,
	CHAR_SQUOTE,
	CHAR_DQUOTE,
	CHAR_NORMAL
}								t_char_type;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_UNKNOWN,
	TOKEN_SPACE,
	TOKEN_SQUOTE,
	TOKEN_DQUOTE,
	TOKEN_ENV
}								t_token_type;

/*STRUCTS*/
typedef struct s_node
{
	char						*command;
	char						*path;
	char						**args;
	char						**infile;
	char						**outfile;
	char						**heredoc;
	int							*append;
	int							*fd_in;
	int							*fd_out;
	int							n_input;
	int							n_output;
}								t_node;

typedef struct s_token
{
	char						*value;
	int							type;
	struct s_token				*next;
}								t_token;

typedef struct s_command
{
	char						**args;
	char						*infile;
	char						*outfile;
	int							in_type;
	int							out_type;
	bool						is_valid;
	struct s_command			*next;
	struct s_command			*prev;
}								t_command;

typedef struct s_shell
{
	char						**envp;
	t_command					*cmnd_lst;
	t_token						*tokens;
	int							**pipe;
	pid_t						*pid;
	int							n_cmnds;
	int							exit_status;
	bool						running;
}								t_shell;

/*MAIN*/
void							initialize_shell(t_shell *shell, char **envp);
int								prevent_batch_and_init(t_shell *shell,
									char **env, int argc, char **argv);
void							run_shell_loop(t_shell *shell);
void							process_shell_input(char *input,
									t_shell *shell);
void							reset_shell_state(t_shell *shell);

/*TOKENIZER*/
char							*ft_strjoin_free(char *s1, char *s2);
t_token							*create_token(const char *value,
									t_token_type type);
t_token							*tokenize_input(const char *input,
									t_shell *shell);
t_token							*tokenize_word(const char *input, int *i,
									t_shell *shell);
t_token							*tokenize_pipe(const char *input, int *pos);
t_token							*tokenize_quoted_str(char *input, int *i,
									t_shell *shell);
t_token							*get_token_type(const char *input, int *pos,
									t_shell *shell);
void							skip_whitespace(const char *input, int *pos);

/*TOKENIZER /EXTRACT WORD*/
char							*append_word_part(char *result,
									const char *input, int start, int len);
char							*process_quoted_segment(char *result,
									const char *input, int *start, int *len);
char							*extract_quoted(char *input, int *pos,
									char quote_type);
char							*extract_word(const char *input, int *pos);
char							*handle_word_part(char *result,
									const char *input, int *start, int *len);

/*TOKENIZER /UTILS*/
t_token_type					get_operator_type(char c);
t_token							*tokenize_single_operator(const char *input,
									int *i);
t_token							*tokenize_double_operator(const char *input,
									int *pos, const char *op_str,
									t_token_type type);

int								handle_quotes(char *input, int *i,
									char quote_type);
char							**insert_arg_array(char **orig_args, int pos,
									char **expanded);
char							*handle_quote_error(char *result);
char							*append_unquoted_part(char *result, const \
								char *input, int *start, int *len);

/*PIPES*/
int								create_pipe(int pipe_fd[2], t_shell *shell);
void							close_pipe_ends(int pipe_fd[2]);
int								setup_pipe_io(int in_fd, int out_fd);
int								get_input_fd(int *prev_pipe);
int								get_output_fd(int *pipe_fd);
void							handle_pipe_io_error(int *prev_pipe,
									int *pipe_fd);

/*PARSER*/
t_command						*parse_command(t_token **token);
int								parse_tokens(t_shell *shell);
bool							validate_command(t_command *cmd,
									t_shell *shell);

/*PARSER /UTILS*/
t_command						*create_cmd_node(void);
void							add_cmd_node(t_command **cmd_list,
									t_command *new_cmd);
void							cleanup_cmd_list(t_command *cmd);
int								parse_cmd_arguments(t_token **token,
									t_command *cmd);
int								parse_redirections(t_token **token,
									t_command *cmd);
char							**add_cmd_argument(char **args, char *new_arg);
char							**add_new_argument(char **new_args,
									char *new_arg, int i);
char							**create_new_array(char **args, int count);
char							**copy_existing_args(char **new_args,
									char **args, int *i);

/*PARSING /ENV_EXPANSION*/
char							*extract_env_var_name(const char *str);
char							*expand_env_vars(char *str, t_shell *shell);

/*EXECUTOR*/
int								execute_commands(t_shell *shell);

/*BUILTINS*/
int								ft_cd(char **args, t_shell *shell);
int								ft_pwd(char **args, t_shell *shell);
int								ft_echo(char **args, t_shell *shell);
int								ft_export(char **args, t_shell *shell);
int								ft_unset(char **args, t_shell *shell);
int								ft_env(char **args, t_shell *shell);
int								check_env_args(char **args);
int								ft_exit(char **args, t_shell *shell);

/*CLEANUP*/
void							cleanup_execution_data(t_shell *shell);
void							cleanup_envp(t_shell *shell);
void							cleanup_token_list(t_token *tokens);
void							cleanup_fd_arrays(t_node *node);
void							cleanup_file_resources(t_node *node);
void							cleanup_cmd_node(t_node *node);
void							cleanup_shell_data(t_shell *shell);
void							cleanup_cmd_list(t_command *cmd);
int								cleanup_and_exit(char *err_msg, char *src,
									int err_code, t_shell *shell);

/*UTILS*/
t_char_type						find_special_chars(char c);
char							*handle_escape(int *i);

/*UTILS /CHECKERS*/
int								ft_is_whitespace(char c);
int								is_quote(char c);
int								is_special_char(char c);
int								is_word_delimiter(char c);

/*UTILS ERRORS*/
void							print_command_error(char *cmd, char *error_msg);
void							print_redir_error(char *msg, char *file);
int								print_syntx_err(char *err_msg, char *src);
void							print_error_msg(char *err_msg, char *src);
void							print_file_error(const char *filename,
									const char *error_msg);

/*UTILS /SIGNALS*/
void							setup_signals(void);
void							setup_child_signal(void);
void							handle_eof(t_shell *shell);
void							interactive_signal_handler(int signum);
void							setup_execution_signals(struct sigaction \
								*sa_old_int, struct sigaction *sa_old_quit);
int								setup_heredoc_signals(void);
/*UTILS /SHELL*/
void							run_shell_loop(t_shell *shell);
void							initialize_shell(t_shell *shell, char **envp);
int								validate_env_var(char *name);

/*ENVIRONMENT*/
char							*get_env_value(char *name, t_shell *shell);
int								count_env_vars(char **envp);
int								update_env_value(char *name, char *value,
									t_shell *shell);
int								remove_env_var(char *name, t_shell *shell);

/*ENVIRONMENT /UTILS*/

/*ENVIRONMENT /UTILS*/
char							*get_home_dir(t_shell *shell);
int								update_pwd_vars(t_shell *shell);
// int				has_equals_sign(char *str);
char							*get_env_value(char *name, t_shell *shell);
int								find_env_index(char *name, char **envp);
char							*create_env_string(char *name, char *value);
int								update_env_value(char *name, char *value,
									t_shell *shell);
char							**copy_env(char **envp);
int								add_env_var(char *arg, t_shell *shell);
void							update_shell_level(t_shell *shell);
void							print_sorted_env(char **envp);

/*BUILTINS /ENV*/
int								ft_env(char **args, t_shell *shell);

/*BUILTINS /UTILS*/
int								is_valid_n_flag(char *arg);

/*BUILTINS /EXECUTION*/
int								handle_builtin_cmd(t_command *cmd,
									t_shell *shell);

/*BUILTINS /CD*/
char							*get_home_dir(t_shell *shell);
char							*expand_path(char *path, t_shell *shell);
int								update_pwd_vars(t_shell *shell);
char							*expand_path(char *path, t_shell *shell);

/*BUILTINS /EXPORT*/
int								validate_env_var(char *name);
char							*extract_env_var_value(char *arg);
int								update_existing_var(char *arg, t_shell *shell);
int								add_new_var(char *arg, t_shell *shell);
char							**copy_env(char **envp);
void							print_exported_var(char *var);

/*BUILTINS /UNSET*/
int								validate_env_var(char *name);
// void			print_identifier_error(char *arg);

/*EXECUTOR /REDIRECTIONS*/
int								setup_redirections(t_command *cmd);
void							restore_std_fds(int stdin_fd, int stdout_fd);
int								redirect_output(int fd, char *outfile);
int								backup_std_fds(int *stdin_backup, \
								int *stdout_backup);
int								open_output_file(char *outfile, int flags);

/*EXECUTOR /PIPELINE*/
int								setup_pipeline_steps(t_command *current,
									int *prev_pipe, pid_t *last_pid,
									t_shell *shell);
void							cleanup_pipeline_resources(int *prev_pipe,
									int *pipe_fd);

/*EXECUTOR /PROCESSES*/
int								create_process(pid_t *pid, t_shell *shell);
void							handle_pipeline_child(t_command *cmd,
									int *prev_pipe, int *pipe_fd,
									t_shell *shell);
void							handle_parent_process(int *prev_pipe,
									int *pipe_fd);
void							wait_for_children(pid_t last_pid);

/*EXECUTOR /UTILS*/
int								is_builtin(char *cmd);
int								handle_builtin_cmd(t_command *cmd,
									t_shell *shell);
char							*get_cmd_path(char *cmd, char **paths);
char							*find_command_path(char *cmd, t_shell *shell);

/*EXECUTOR /EXEC*/
int								execute_single_command(t_command *cmd,
									t_shell *shell);
int								execute_single_builtin(t_command *cmd,
									t_shell *shell);
int								is_parent_only_builtin(char *cmd);
int								setup_pipeline_steps(t_command *current,
									int *prev_pipe, pid_t *last_pid,
									t_shell *shell);
void							handle_pipeline_child(t_command *cmd,
									int *prev_pipe, int *pipe_fd,
									t_shell *shell);
void							handle_parent_process(int *prev_pipe,
									int *pipe_fd);
void							wait_for_children(pid_t last_pid);
int								setup_heredoc(t_command *cmd);

/*EXECUTOR /EXTERNAL AND UTILS*/
int								handle_external_cmd(t_command *cmd,
									t_shell *shell);
void							handle_wait_status(int status);
int								handle_wildcard_expansion(t_command *cmd,
									t_shell *shell);
int								execute_external_single_cmd(t_command *cmd,
									char *cmd_path, t_shell *shell);
int								print_command_not_found(t_command *cmd);
void							execute_pipeline_cmd(t_command *cmd,
									char *cmd_path, t_shell *shell);
void							handle_command_errors(char *cmd_path,
									char *cmd_name);
void							execute_external_child(t_command *cmd,
									char *cmd_path, t_shell *shell);

#endif

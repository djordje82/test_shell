# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/05 14:34:50 by j                 #+#    #+#              #
#    Updated: 2024/12/05 14:28:53 by dodordev         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Program name
NAME = minishell

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
INCLUDES = -I./includes -I./libft
LIBS = -lreadline -L$(LIBFT_DIR) -lft

# Directories
LIBFT_DIR = ./libft
SRCS_DIR = srcs
OBJS_DIR = objs

# Source files
SRCS =	main.c \
		builtins/ft_cd.c \
		builtins/ft_echo.c \
		builtins/ft_env.c \
		builtins/ft_exit.c \
		builtins/ft_export.c \
		builtins/ft_pwd.c \
		builtins/ft_unset.c \
		executor/exec.c \
		executor/setup_redirections.c \
		executor/exec_utils.c \
		executor/exec_external.c \
		executor/exec_external_utils.c \
		executor/exec_single_cmd.c \
		executor/exec_builtins.c \
		executor/exec_pipeline.c \
		executor/exec_pipeline_utils.c \
		executor/heredoc.c \
		utils/cleanup/cleanup_shell.c \
		utils/cleanup/cleanup_shell_utils.c \
		utils/cleanup/cleanup_cmd_node.c \
		env/env_modify.c \
		env/env_core.c \
		env/env_print.c \
		env/env_parse.c \
		utils/errors/print_errors.c \
		utils/utils.c \
		utils/checkers.c \
		parser/parser.c \
		parser/parser_utils.c \
		parser/parse_redirections.c \
		parser/command_utils.c \
		parser/env_expansion.c \
		signals/signals.c \
		signals/sig_utils.c \
		tokenizer/tokenizer.c \
		tokenizer/tokenize_quoted.c \
		tokenizer/tokenizer_utils.c \
		tokenizer/tokenize_operators.c \
		tokenizer/tokenize_word.c \
		tokenizer/extract_word.c \
		pipes/pipe_manager.c \
		pipes/pipe_manager_utils.c
# Object files
OBJS = $(SRCS:%.c=$(OBJS_DIR)/%.o)

# Libft
LIBFT = $(LIBFT_DIR)/libft.a

# Colors for output
GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m

# Default target
all: $(NAME)

# Create object directories
$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)
	@mkdir -p $(OBJS_DIR)/execution/builtins
	@mkdir -p $(OBJS_DIR)/execution/executor
	@mkdir -p $(OBJS_DIR)/utils/cleaning
	@mkdir -p $(OBJS_DIR)/utils/errors
	@mkdir -p $(OBJS_DIR)/utils/signals
	@mkdir -p $(OBJS_DIR)/parsing/parser
	@mkdir -p $(OBJS_DIR)/parsing/tokenizer

# Compile libft
$(LIBFT):
	@echo "$(GREEN)Compiling libft...$(RESET)"
	@make -C $(LIBFT_DIR)

# Compile object files
$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Link the program
$(NAME): $(OBJS_DIR) $(LIBFT) $(OBJS)
	@echo "$(GREEN)Linking minishell...$(RESET)"
	@$(CC) $(OBJS) $(LIBS) -o $(NAME)
	@echo "$(GREEN)minishell is ready!$(RESET)"

# Clean object files
clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@rm -rf $(OBJS_DIR)
	@make -C $(LIBFT_DIR) clean

# Clean everything
fclean: clean
	@echo "$(RED)Cleaning everything...$(RESET)"
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

# Rebuild everything
re: fclean all

# Debug target
debug: CFLAGS += -g
debug: re

# Phony targets
.PHONY: all clean fclean re debug


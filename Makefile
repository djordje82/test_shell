# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/05 14:34:50 by j                 #+#    #+#              #
#    Updated: 2024/11/08 12:15:12 by dodordev         ###   ########.fr        #
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
		execution/builtins/ft_cd.c \
		execution/builtins/ft_echo.c \
		execution/builtins/ft_env.c \
		execution/builtins/ft_exit.c \
		execution/builtins/ft_export.c \
		execution/builtins/ft_pwd.c \
		execution/builtins/ft_unset.c \
		execution/executor/exec.c \
		execution/executor/setup_redirections.c \
		execution/executor/exec_utils.c \
		execution/executor/exec_external.c \
		execution/executor/exec_external_utils.c \
		execution/executor/exec_single_cmd.c \
		execution/executor/exec_builtins.c \
		execution/executor/exec_pipeline.c \
		execution/executor/processes.c \
		execution/executor/heredoc.c \
		utils/cleanup/cleanup_shell.c \
		utils/cleanup/cleanup_cmd_node.c \
		utils/cleanup/cleanup_utils.c \
		utils/env/env_modify.c \
		utils/env/env_utils.c \
		utils/errors/print_errors.c \
		utils/signals/signals.c \
		utils/signals/sig_utils.c \
		utils/args_utils.c \
		utils/utils.c \
		utils/checkers.c \
		parsing/parser.c \
		parsing/parser_utils.c \
		parsing/parse_redirections.c \
		parsing/command_utils.c \
		parsing/wildcards.c \
		parsing/wildcards_utils.c \
		parsing/env_expansion.c \
		tokenizer/tokenizer.c \
		tokenizer/tokenize_quoted.c \
		tokenizer/tokenizer_utils.c \
		tokenizer/tokenize_operators.c \
		tokenizer/tokenize_word.c \
		tokenizer/extract_word.c 

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


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
		execution/executor/executor.c \
		utils/cleaning/clean.c \
		utils/cleaning/clean_utils.c \
		utils/errors/error.c \
		utils/signals/signals.c \
		utils/signals/sig_utils.c \
		utils/utils.c \
		parsing/parser/parser.c \
		parsing/parser/parser_utils.c \
		parsing/parser/command_utils.c \
		parsing/tokenizer/tokenizer.c \
		parsing/tokenizer/tokenizer_utils.c \
		parsing/tokenizer/tokenizer_operators.c \
		execution/executor/redirections.c \
		execution/executor/executor_utils.c \
		execution/executor/external.c

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


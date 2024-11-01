# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dodordev <dodordev@student.42berlin.de>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/05 14:34:50 by j                 #+#    #+#              #
#    Updated: 2024/10/24 12:20:20 by dodordev         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = minishell

SRCS =	srcs/main.c \
		srcs/execution/builtins/ft_cd.c \
		srcs/execution/builtins/ft_echo.c \
		srcs/execution/builtins/ft_env.c \
		srcs/execution/builtins/ft_exit.c \
		srcs/execution/builtins/ft_export.c \
		srcs/execution/builtins/ft_pwd.c \
		srcs/execution/builtins/ft_unset.c \
		srcs/execution/executor/executor.c \
		srcs/utils/cleaning/clean.c \
		srcs/utils/cleaning/clean_utils.c \
		srcs/utils/errors/errors.c \
		srcs/utils/signals/signals.c \
		srcs/utils/signals/sig_utils.c \
		srcs/utils/utils.c \
		srcs/parsing/parser/parser.c \
		srcs/parsing/parser/parser_utils.c \
		srcs/parsing/parser/command_utils.c \
		srcs/parsing/tokenizer/tokenizer.c \
		srcs/parsing/tokenizer/tokenizer_utils.c \
		srcs/parsing/tokenizer/tokenizer_operators.c \
OBJS = $(SRCS:.c=.o)

CFLAGS = -Wall -Wextra -Werror -g -I./include
LIBS = -lreadline

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

.PHONY: all clean fclean re

all: $(LIBFT) $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBS) $(LIBFT)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all


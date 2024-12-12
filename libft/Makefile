# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jadyar <jadyar@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/30 12:10:30 by dodordev          #+#    #+#              #
#    Updated: 2024/12/12 11:08:28 by jadyar           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

LIBFT = Libft/libft.a

CC = cc

CCFLAGS = -Wall -Wextra -Werror

ARNAME = ar crs $(NAME)

INCLUDES = -I inc

RM = rm -f

MKDIR_P = mkdir -p

COMPILATION_FLAG_FILE=.compilation_started

#                                    LIBFT                                     #

SRC_DIR = src/

OBJS_DIR = objs/

LIBFT_FILES = isalpha isdigit isalnum isnumber \
		isascii isprint atoi atoll is_whitespace \
		bzero calloc itoa count_args \
		close_fd \
		memchr memcmp memcpy \
		memmove memset \
		free_array \
		putchar_fd putendl_fd putnbr_fd \
		putstr_fd split strchr \
		strdup striteri strjoin strjoin_free \
		strlcat strlcpy strlen \
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
		strmapi strncmp strcmp strnstr \
=======
		strmapi strncmp strnstr strncat strncpy \
>>>>>>> 936e186 (quoted issue in tokenizer resolved)
=======
		strmapi strncmp strnstr strncat strncpy \
>>>>>>> 936e186 (quoted issue in tokenizer resolved)
=======
		strmapi strncmp strnstr strncat strncpy \
>>>>>>> 936e186 (quoted issue in tokenizer resolved)
		strrchr strtrim substr \
		tolower toupper\
		lstnew lstadd_front lstsize \
		lstlast lstadd_back lstdelone \
		lstclear lstiter lstmap \

SRC = $(addprefix $(SRC_DIR)ft_, $(addsuffix .c, $(LIBFT_FILES)))

OBJS := $(addprefix $(OBJS_DIR)ft_, $(addsuffix .o, $(LIBFT_FILES)))

#                                    PRINTF                                    #

PRINTF_SRC_DIR = $(SRC_DIR)printf

PRINTF_OBJS_DIR = $(OBJS_DIR)printf/

PRINTF_FILES = ft_printf ft_print_c ft_print_d \
		ft_print_p ft_print_s ft_print_u ft_print_x

PRINTF_SRC = $(addprefix $(PRINTF_SRC_DIR)/, $(addsuffix .c, $(PRINTF_FILES)))

PRINTF_OBJS := $(addprefix $(PRINTF_OBJS_DIR)/, $(addsuffix .o, $(PRINTF_FILES)))

#                                       GNL                                    #

GNL_SRC_DIR = $(SRC_DIR)gnl

GNL_OBJS_DIR = $(OBJS_DIR)gnl/

GNL_FILES = get_next_line get_next_line_utils

GNL_SRC = $(addprefix $(GNL_SRC_DIR), $(addsuffix .c, $(GNL_FILES)))

GNL_OBJS := $(addprefix $(GNL_OBJS_DIR), $(addsuffix .o, $(GNL_FILES)))

# **************************************************************************** #
#                                 LIBFT RULES                                  #
# **************************************************************************** #

all: libft_std printf gnl

libft_std: $(NAME)

$(NAME): directories $(OBJS)
	$(call print_status,"Creating library $(NAME)...")
	@$(ARNAME) $(OBJS) > /dev/null
	@echo "${CHECK} Library $(NAME) created"
	@$(RM) $(COMPILATION_FLAG_FILE)

$(OBJS_DIR)%.o: $(SRC_DIR)%.c
	@if [ ! -f $(COMPILATION_FLAG_FILE) ]; then \
		echo "${PINK}====> Compiling library${NC}"; \
		touch $(COMPILATION_FLAG_FILE); \
	fi
	@$(CC) $(INCLUDES) $(CCFLAGS) -c $< -o $@ > /dev/null
	@echo "${CHECK} Compiled $<"

$(OBJS_DIR):
	@$(MKDIR_P) $(OBJS_DIR)

# **************************************************************************** #
#                                PRINTF RULES                                  #
# **************************************************************************** #

printf: $(NAME) $(PRINTF_OBJS)
	$(call print_status,"Adding Printf to $(NAME)...")
	@$(ARNAME) $(PRINTF_OBJS) > /dev/null
	@echo "${CHECK} Library $(NAME) created"
	@$(RM) $(COMPILATION_FLAG_FILE)

$(PRINTF_OBJS_DIR): $(OBJS_DIR)
	@$(MKDIR_P) $(PRINTF_OBJS_DIR)


$(PRINTF_OBJS_DIR)%.o: $(PRINTF_SRC_DIR)%.c
	@$(CC) $(INCLUDES) $(CCFLAGS) -c $< -o $@ > /dev/null
	@echo "${CHECK} Compiled $<"

# **************************************************************************** #
#                                  GNL RULES                                   #
# **************************************************************************** #

gnl: $(NAME) $(GNL_OBJS)
	$(call print_status,"Adding GNL to $(NAME)...")
	@$(ARNAME) $(GNL_OBJS) > /dev/null
	@echo "${CHECK} Library $(NAME) created"
	@$(RM) $(COMPILATION_FLAG_FILE)

$(GNL_OBJS_DIR): $(OBJS_DIR)
	@$(MKDIR_P) $(GNL_OBJS_DIR)

$(GNL_OBJS_DIR)%.o: $(GNL_SRC_DIR)%.c
	@$(CC) $(INCLUDES) $(CCFLAGS) -c $< -o $@ > /dev/null
	@echo "${CHECK} Compiled $<"

# **************************************************************************** #
#                                    RULES                                     #
# **************************************************************************** #

directories: $(OBJS_DIR) $(GNL_OBJS_DIR) $(PRINTF_OBJS_DIR)

clean:
	$(call print_status,"Cleaning up object files...")
	@$(RM) $(OBJS) $(OBJS_PRINTF) $(OBJS_GNL) $(COMPILATION_FLAG_FILE)
	@rm -rf $(OBJS_DIR)
	@echo "${CHECK} Cleanup complete"

fclean: clean
	$(call print_status,"Full clean - Removing library...")
	@$(RM) $(NAME)
	@echo "${CHECK} Full cleanup complete"

re: fclean all

.PHONY: all printf gnl libft_std directories clean fclean re
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ykhomsi <ykhomsi@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/21 16:47:20 by ykhomsi           #+#    #+#              #
#    Updated: 2025/04/03 16:42:49 by ykhomsi          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -I includes
RM = rm -rf

# Directories
OBJ_DIR = objs

# Source files
SRCS = philosophers.c \
       init.c \
       routine.c \
       routine_utils.c \
       utils.c \
       utils2.c \
       cleanup.c

# Object files
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

# Colors
GREEN = \033[0;32m
YELLOW = \033[0;33m
RESET = \033[0m

all: $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@echo "$(GREEN)Created objects directory$(RESET)"

$(OBJ_DIR)/%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(YELLOW)Compiling:$(RESET) $<"

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)Philosophers compiled successfully!$(RESET)"
	@echo "$(YELLOW)"
	@echo "███████╗ █████╗ ████████╗██╗███╗   ██╗ ██████╗     ██████╗ ██╗  ██╗██╗██╗      ██████╗ ███████╗ ██████╗ ██████╗ ██╗  ██╗███████╗██████╗ ███████╗"
	@echo "██╔════╝██╔══██╗╚══██╔══╝██║████╗  ██║██╔════╝     ██╔══██╗██║  ██║██║██║     ██╔═══██╗██╔════╝██╔═══██╗██╔══██╗██║  ██║██╔════╝██╔══██╗██╔════╝"
	@echo "█████╗  ███████║   ██║   ██║██╔██╗ ██║██║  ███╗    ██████╔╝███████║██║██║     ██║   ██║███████╗██║   ██║██████╔╝███████║█████╗  ██████╔╝███████╗"
	@echo "██╔══╝  ██╔══██║   ██║   ██║██║╚██╗██║██║   ██║    ██╔═══╝ ██╔══██║██║██║     ██║   ██║╚════██║██║   ██║██╔═══╝ ██╔══██║██╔══╝  ██╔══██╗╚════██║"
	@echo "███████╗██║  ██║   ██║   ██║██║ ╚████║╚██████╔╝    ██║     ██║  ██║██║███████╗╚██████╔╝███████║╚██████╔╝██║     ██║  ██║███████╗██║  ██║███████║"
	@echo "╚══════╝╚═╝  ╚═╝   ╚═╝   ╚═╝╚═╝  ╚═══╝ ╚═════╝     ╚═╝     ╚═╝  ╚═╝╚═╝╚══════╝ ╚═════╝ ╚══════╝ ╚═════╝ ╚═╝     ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚══════╝"
	@echo "$(RESET)"

clean:
	@$(RM) $(OBJ_DIR)
	@echo "$(GREEN)Cleaned object files$(RESET)"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(GREEN)Cleaned executable$(RESET)"

re: fclean all

.PHONY: all clean fclean re
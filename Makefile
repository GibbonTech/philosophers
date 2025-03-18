# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ykhomsi <ykhomsi@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/18 03:02:21 by ykhomsi           #+#    #+#              #
#    Updated: 2025/03/18 03:02:36 by ykhomsi          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iincludes

all: $(NAME)

SRCS = philosophers.c srcs/validation.c srcs/string_conversion.c srcs/init.c srcs/philosopher_actions.c srcs/philosopher_routine.c srcs/cleanup.c srcs/time_utils.c srcs/thread_start.c

OBJS = $(SRCS:.c=.o)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	rm -f $(NAME)

fclean: clean

re: fclean all

NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -I includes
RM = rm -f

SRCS = philosophers.c \
       init.c \
       routine.c \
       utils.c \
       utils2.c \
       cleanup.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re 
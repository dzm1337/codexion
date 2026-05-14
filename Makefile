NAME = codexion
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread
RM = rm -f

SRCS = main.c \
       src/coder_actions.c \
       src/dongle.c \
       src/dongle_init_utils.c \
       src/edf_scheduler.c \
       src/fifo_scheduler.c \
       src/init.c \
       src/routine.c \
       src/utils.c \
			 src/wait_condition.c \
			 src/init_threads.c \
			 src/parser.c \

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re

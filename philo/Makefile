NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
AR = ar rcs
RM = rm -f

SRC = philo.c init_structures.c philo_utils.c routines.c
OBJ = $(SRC:.c=.o)
HEADER = philo.h

all: $(NAME)

$(NAME): $(OBJ) $(HEADER)
	$(CC) $(CFLAGS) $(OBJ) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re

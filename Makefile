CC=c++
FLAGS=-Werror -Wall -Wextra -g3
SRCS=main.cpp
OBJS=$(SRCS:.cpp=.o)
NAME=ft_irc

.PHONY: all start $(NAME)

all: start $(NAME)

start:
	@echo "\e[1;33m===============FT_IRC==============="

$(NAME): $(OBJS) $(HEADER)
	@echo "\e[1;32m [LIBFT] Building ..."
	@$(CC) $(FLAGS) $(OBJS) -o $(NAME)
	@echo "\e[1;32m [LIBFT] Build Complete !"

%.o: %.c
	@echo "\e[1;32m [LIBFT] Compiling $^"
	@$(CC) $(FLAGS) $^ -c -o $@

fclean: clean
	@echo "\e[1;31m [DELETING LIBRARY]"
	@rm -f $(NAME)

clean:
	@echo "\e[1;31m [DELETING RESIDUAL FILES]"
	@rm -f $(OBJS)

re: clean all

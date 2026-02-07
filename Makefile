CC=c++
FLAGS=-Werror -Wall -Wextra -g3
SRCS=main.cpp Server.cpp Client.cpp Channel.cpp RPL.cpp MSG.cpp CMD.cpp Package.cpp Ft.cpp
OBJS=$(SRCS:.cpp=.o)
NAME=ft_irc

.PHONY: all start $(NAME)

all: start $(NAME)

start:
	@echo "\e[1;33m===============FT_IRC==============="

$(NAME): $(OBJS)
	@echo "\e[1;32m [FT_IRC] Building ..."
	@$(CC) $(FLAGS) $(OBJS) -o $(NAME)
	@echo "\e[1;32m [FT_IRC] Build Complete !"

%.o: %.cpp
	@echo "\e[1;32m [FT_IRC] Compiling $^"
	@$(CC) $(FLAGS) $^ -c -o $@

fclean: clean
	@echo "\e[1;31m [DELETING EXECUTABLE]"
	@rm -f $(NAME)

clean:
	@echo "\e[1;31m [DELETING RESIDUAL FILES]"
	@rm -f $(OBJS)

re: clean all

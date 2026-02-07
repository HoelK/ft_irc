CC=c++
FLAGS=-Werror -Wall -Wextra -g3
SRCS=srcs/main.cpp \
	 srcs/Server.cpp \
	 srcs/Client.cpp \
	 srcs/Channel.cpp \
	 srcs/RPL.cpp \
	 srcs/MSG.cpp \
	 srcs/CMD.cpp \
	 srcs/Package.cpp \
	 srcs/Ft.cpp
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
	@$(CC) $(FLAGS) $^ -c -o $@ -I./headers/

fclean: clean
	@echo "\e[1;31m [DELETING EXECUTABLE]"
	@rm -f $(NAME)

clean:
	@echo "\e[1;31m [DELETING RESIDUAL FILES]"
	@rm -f $(OBJS)

re: clean all

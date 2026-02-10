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
	@echo "\e[1;33m===============FT_IRC===============\033[0m"

$(NAME): $(OBJS)
	@echo "\e[1;32m [FT_IRC] Building ...\033[0m"
	@$(CC) $(FLAGS) $(OBJS) -o $(NAME)
	@echo "\e[1;32m [FT_IRC] Build Complete !\033[0m"

%.o: %.cpp
	@echo "\e[1;32m [FT_IRC] Compiling $^\033[0m"
	@$(CC) $(FLAGS) $^ -c -o $@ -I./headers/

fclean: clean
	@echo "\e[1;31m [DELETING EXECUTABLE]\033[0m"
	@rm -f $(NAME)

clean:
	@echo "\e[1;31m [DELETING RESIDUAL FILES]\033[0m"
	@rm -f $(OBJS)

re: clean all

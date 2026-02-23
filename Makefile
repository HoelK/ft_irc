CC=c++
FLAGS=-Werror -Wall -Wextra -std=c++98 -g3
SRCS=srcs/main.cpp \
	 srcs/Server.cpp \
	 srcs/Client.cpp \
	 srcs/Channel.cpp \
	 srcs/RPL.cpp \
	 srcs/MSG.cpp \
	 srcs/Package.cpp \
	 srcs/Ft.cpp \
	 srcs/cmds/CMD.cpp \
	 srcs/cmds/Mode.cpp \
	 srcs/cmds/Join.cpp \
	 srcs/cmds/Invite.cpp \
	 srcs/cmds/Kick.cpp \
	 srcs/cmds/Topic.cpp \
	 srcs/cmds/Priv.cpp \
	 srcs/cmds/Nick.cpp \
	 srcs/cmds/User.cpp \
	 srcs/Errors.cpp
OBJS=$(SRCS:.cpp=.o)
NAME=ircserv

.PHONY: all start $(NAME)

all: start $(NAME)

start:
	@echo "\e[1;33m===============FT_IRC===============\033[0m"

$(NAME): $(OBJS)
	@printf "\e[1;32m [FT_IRC] Building ...\n\033[0m"
	@$(CC) $(FLAGS) $(OBJS) -o $(NAME)
	@printf "\e[1;32m [FT_IRC] Build Complete !\n\033[0m"

%.o: %.cpp
	@printf "\e[1;32m [FT_IRC] Compiling $^\n\033[0m"
	@$(CC) $(FLAGS) $^ -c -o $@ -I./headers/ -I./headers/cmds/

fclean: clean
	@printf "\e[1;31m [DELETING EXECUTABLE]\n\033[0m"
	@rm -f $(NAME)

clean:
	@printf "\e[1;31m [DELETING RESIDUAL FILES]\n\033[0m"
	@rm -f $(OBJS)

re: fclean all

CC=c++
FLAGS=-Werror -Wall -Wextra -std=c++98 -g3
SRC_DIR	:= srcs
OBJ_DIR := obj
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
OBJS=$(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
NAME=ircserv

.PHONY: all start $(NAME)

all: start $(NAME)

start:
	@echo "\e[1;33m===============FT_IRC===============\033[0m"

$(NAME): $(OBJS)
	@printf "\e[1;32m [FT_IRC] Building ...\n\033[0m"
	@$(CC) $(FLAGS) $(OBJS) -o $(NAME)
	@printf "\e[1;32m [FT_IRC] Build Complete !\n\033[0m"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	@printf "\e[1;32m [FT_IRC] Compiling $^\n\033[0m"
	@$(CC) $(FLAGS) $^ -c -o $@ -I./headers/ -I./headers/cmds/

fclean: clean
	@printf "\e[1;31m [DELETING EXECUTABLE]\n\033[0m"
	@rm -f $(NAME)

clean:
	@printf "\e[1;31m [DELETING RESIDUAL FILES]\n\033[0m"
	@rm -rf $(OBJ_DIR)

re: fclean all

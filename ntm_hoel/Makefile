CXX      := c++
CXXFLAGS := -Wall -Wextra -Werror -std=c++98

SRC_DIR := src
OBJ_DIR := obj
NAME := ft_irc

CPP_FILES := $(shell find $(SRC_DIR) -type f -name "*.cpp")
REL_CPP   := $(patsubst ./%,%,$(CPP_FILES))
OBJ_FILES := $(addprefix $(OBJ_DIR)/,$(REL_CPP:.cpp=.o))

all: $(NAME)

$(NAME): $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) -o $@

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

re: clean all

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

.PHONY: all clean re fclean

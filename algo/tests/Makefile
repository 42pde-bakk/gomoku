NAME = test.out
INCLUDE = -I../include -I../include/Gamerules -I../include/IO
HEADER = $(shell find "../include" -type f -name "*.hpp")

SRC_EXT = cpp
OBJ_EXT = o

SRC_DIR = ../srcs
BUILD_DIR = ../obj
SOURCES := $(shell find $(SRC_DIR) -type f -name "*.$(SRC_EXT)" ! -name "*main.cpp")
OBJS    := $(SOURCES:.$(SRC_EXT)=.$(OBJ_EXT))
OBJECTS := $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(OBJS))

TEST_DIR = ./srcs
TEST_BUILD_DIR = ./obj
TEST_SOURCES := $(shell find $(TEST_DIR) -type f -name "*.$(SRC_EXT)")
TEST_OBJS    := $(TEST_SOURCES:.$(SRC_EXT)=.$(OBJ_EXT))
TEST_OBJECTS := $(patsubst $(TEST_DIR)/%,$(TEST_BUILD_DIR)/%,$(TEST_OBJS))


# COLORS
PINK = \x1b[35;01m
BLUE = \x1b[34;01m
YELLOW = \x1b[33;01m
GREEN = \x1b[32;01m
RED = \x1b[31;01m
WHITE = \x1b[31;37m
RESET = \x1b[0m

CFLAGS = -Wall -Werror -Wextra --std=c++17
ifdef DEBUG
 CFLAGS += -g3
endif
ifdef LEAKS
 CFLAGS += -g3 -fsanitize=address
endif

SHELL := /bin/bash
export SHELL
export DEBUG

all: directories $(NAME)

directories:
	@mkdir -p $(BUILD_DIR)

$(NAME): $(TEST_OBJECTS) $(HEADER) $(OBJECTS)
	$(CXX) $(CFLAGS) $(OBJECTS) $(TEST_OBJECTS) -o $(NAME)
	@printf "$(PINK)Done building $(NAME) $(RESET)\n"

$(TEST_BUILD_DIR)/%.o: $(TEST_DIR)/%.$(SRC_EXT)
	@mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) -c $(INCLUDE) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.$(SRC_EXT)
	@mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) -c $(INCLUDE) $^ -o $@

clean:
	/bin/rm -f $(TEST_OBJECTS)

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all

dirs:
	@mkdir $(patsubst %, $(PATH_OBJ)/%, $(DIRS)) $(patsubst %, bin/%, $(DIRS))

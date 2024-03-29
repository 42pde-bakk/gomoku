NAME = gomokubot
INCLUDE = -Iinclude -Iinclude/Gamerules -Iinclude/IO -Iinclude/Async
HEADER = $(shell find "include" -type f -name "*.hpp")

SRC_DIR = srcs
BUILD_DIR = obj
SRC_EXT = cpp
OBJ_EXT = o

SOURCES := $(shell find $(SRC_DIR) -type f -name "*.$(SRC_EXT)")
OBJS    := $(SOURCES:.$(SRC_EXT)=.$(OBJ_EXT))
OBJECTS := $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(OBJS))


# COLORS
PINK = \x1b[35;01m
BLUE = \x1b[34;01m
YELLOW = \x1b[33;01m
GREEN = \x1b[32;01m
RED = \x1b[31;01m
WHITE = \x1b[31;37m
RESET = \x1b[0m

CFLAGS = -Wall -Werror -Wextra --std=c++2a -pthread

ifdef DEBUG
 CFLAGS += -g3
endif
ifdef LEAKS
 CFLAGS += -g3 -fsanitize=address
endif
ifdef PROFILING
 CFLAGS += -pg
endif
ifdef THREAD_DEBUG
 CFLAGS += -g3 -fsanitize=thread
endif
ifdef SINGLETHREADED
 CFLAGS += -D THREADED=0
endif
ifndef NO_OPTIMIZATION
 CFLAGS += -Ofast -fno-exceptions -D NDEBUG
endif

SHELL := /bin/bash
export SHELL
export DEBUG

all: $(NAME)

directories:
	@mkdir -p $(BUILD_DIR)

$(NAME): $(OBJECTS) $(HEADER)
	$(CXX) $(CFLAGS) $(OBJECTS) -o $(NAME)
	@printf "$(PINK)Done building $(NAME) $(RESET)\n"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.$(SRC_EXT)
	@mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) -c $(INCLUDE) $^ -o $@

clean:
	/bin/rm -f $(OBJECTS)

fclean: clean
	/bin/rm -f $(NAME)

re:
	$(MAKE) fclean
	$(MAKE) all

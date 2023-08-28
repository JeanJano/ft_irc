CC				:=	g++
CPPFLAGS		:=	-I./include -Wall -Wextra -std=c++98 -MMD -MP -g3

# Directories
SRC_DIR			:=	src/
OBJ_DIR			:=	obj

# Source files
SRCS			:=	$(wildcard $(SRC_DIR)/*.cpp)
OBJS			:=	$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
TARGET			:=	ircserv

# Progress bar function
define progress_bar
		@echo "\033[1;34mCompiling:\033[0m\n"
		@$(1)
		@sleep 0.2
		@echo "\033[1;32mDone\033[0m"
endef

# Targets
all: $(TARGET)

$(TARGET): $(OBJS)
		$(call progress_bar,$(CC) $^ -o $(BIN_DIR)$@)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
		@$(CC) $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR):
		@mkdir -p $(OBJ_DIR)

clean:
		rm -rf $(OBJ_DIR)

fclean: clean
		rm -f $(TARGET)

re: fclean all

.PHONY: all clean fclean re
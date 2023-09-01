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
		@echo "\033[1;34m$<\033[0m\r"
		@$(CC) $(CPPFLAGS) -c $< -o $@
endef

# Targets
all: $(TARGET)

$(TARGET): $(OBJS)
		@echo "\n\033[1;34mLinking...\033[0m"
		@$(CC) $^ -o $@
		@echo "\033[1;32mDone linking: $(TARGET)\033[0m"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
		$(call progress_bar)

$(OBJ_DIR):
		@mkdir -p $(OBJ_DIR)

clean:
		@echo "\033[1;31mRemoving object files: $(OBJ_DIR)\033[0m"
		@rm -rf $(OBJ_DIR)

fclean: clean
		@echo "\033[1;31mRemoving executable: $(TARGET)\033[0m\n"
		@rm -f $(TARGET)

re: fclean all

.PHONY: all clean fclean re
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/04 15:04:46 by rsaueia-          #+#    #+#              #
#    Updated: 2025/01/08 13:22:35 by jparnahy         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Name of the program
NAME = minishell

# Directories
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include

# Subdirectories on the source directory
SUBDIRS = prompt parser exec pipes redirections splitter builtins \
	env expander heredoc utils free from_libft

# Source files
SRC_FILES = $(shell find $(SRC_DIR) -type f -name "*.c")

# Objects management
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Header inclusion
INCLUDES = -I$(INC_DIR) -I/usr/include/readline

# Removing variable
RM = rm -f

# Flags for the compilation
FLAGS = -g -Wall -Wextra -Werror

# Colors for the compilation
RESET = \033[0m
GREEN = \033[1;32m
GREEN_MARK = \033[0;42m
YELLOW = \033[1;33m
BLUE = \033[1;34m
RED = \033[1;31m
RED_MARK = \033[0;41m
CYAN = \033[1;36m
BOLD = \033[1m
ITALIC = \033[3m

# Emojis
CHECK = ✅
CROSS = ❌
ARROW = ➜
GEAR = ⚙️
BOX = 📦

# Main rules
all: $(NAME)

# Linking rule
$(NAME): $(OBJ_FILES)
	@echo
	@echo "$(CYAN)$(GEAR) Linking...$(RESET)"
	$(CC) $(FLAGS) $(OBJ_FILES) -o $(NAME) $(INCLUDES) -lreadline
	@echo
	@echo "$(GREEN)$(CHECK) Compilation complete!$(RESET)"
	@echo "$(YELLOW)Flags used: $(FLAGS)$(RESET)"
	@echo

# Compiling each file .c to .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(addprefix $(OBJ_DIR)/, $(SUBDIRS))
	@mkdir -p $(dir $@)
	@printf "$(BLUE)$(ARROW) Compiling: %-35s$(RESET)\n" "$<"
	$(CC) $(FLAGS) -c $< -o $@ $(INCLUDES)

# Rule to compile all directories (Criar subdiretórios)
$(addprefix $(OBJ_DIR)/, $(SUBDIRS)):
	@mkdir -p $@

# Cleaning rules
clean:
	@echo "$(RED)$(CROSS) Removing object files... $(RESET)"
	$(RM) -r $(OBJ_DIR)
	@echo "$(YELLOW)$(BOX) Object files removed.$(RESET)"
	@echo

fclean: clean
	@echo "$(RED)$(CROSS) Removing executable... $(RESET)"
	$(RM) $(NAME)
	@echo "$(YELLOW)$(GEAR) Executable removed.$(RESET)"
	@echo

re: fclean all

hell: re
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --suppressions=rl.supp ./minishell

.PHONY: all clean fclean re

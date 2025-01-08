# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/04 15:04:46 by rsaueia-          #+#    #+#              #
#    Updated: 2025/01/08 14:20:26 by jparnahy         ###   ########.fr        #
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

# Raiboow colors
RE = \033[38;5;196m
OR = \033[38;5;208m
YE = \033[38;5;226m
GR = \033[38;5;46m
BL = \033[38;5;21m
IN = \033[38;5;93m
VI = \033[38;5;201m

# Emojis
CHECK = ✅
CROSS = ❌
ARROW = ➜
FLGS = 🏳️
GEAR = ⚙️
BOX = 📦

# Barra de progresso
PROGRESS_BAR_LENGTH = 70
PROGRESS_BAR_LENGTH_CLEAN = 40

# Main rules
all: $(NAME)

# Linking rule (com barra de progresso)
$(NAME): $(OBJ_FILES)
	@echo
	@echo "$(CYAN)$(GEAR) Linking...$(RESET)"
	@$(CC) $(FLAGS) $(OBJ_FILES) -o $(NAME) $(INCLUDES) -lreadline & \
	for i in $$(seq 1 $(PROGRESS_BAR_LENGTH)); do \
		printf "$(GREEN_MARK)=$(RESET)"; \
		sleep 0.04; \
	done; \
	echo "$(GREEN)  $(CHECK) 100%$(RESET)"
	@echo
	@echo "$(FLGS)$(YELLOW) Flags used: $(FLAGS)$(RESET)"
	@echo "$(ARROW) $(CYAN) Executable created: ./$(NAME)$(RESET)"
	@echo "$(BOLD)$(GREEN)$(CHECK) Compilation complete!$(RESET)\n"
	@echo
	@echo "$(BOLD)$(RE)W E L $(OR)C O M $(YE)E   T O   $(GR)O U R   \
		$(BL)M I N $(IN)I S H $(VI)E L L$(RESET)\n"
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
	@for i in $$(seq $(PROGRESS_BAR_LENGTH_CLEAN) -1 1); do \
		printf "$(RED_MARK)=$(RESET)"; \
		sleep 0.02; \
	done; \
	$(RM) -r $(OBJ_DIR)
	@echo "$(YELLOW)  $(BOX) Object files removed.$(RESET)"
	@echo

fclean: clean
	@echo "$(RED)$(CROSS) Removing executable... $(RESET)"
	@for i in $$(seq $(PROGRESS_BAR_LENGTH_CLEAN) -1 1); do \
		printf "$(RED_MARK)=$(RESET)"; \
		sleep 0.035; \
	done; \
	$(RM) $(NAME)
	@echo "$(YELLOW)  $(GEAR) Executable removed.$(RESET)"
	@echo

re: fclean all

hell: re
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --suppressions=rl.supp ./minishell

.PHONY: all clean fclean re

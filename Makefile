# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/04 15:04:46 by rsaueia-          #+#    #+#              #
#    Updated: 2024/12/22 13:34:39 by jparnahy         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#Executable name
NAME = minishell

#Directory
SRC_DIR = srcs
OBJS_DIR = objs
INC_DIR = includes

#Specific Directories
BUILTINS_DIR = $(SRC_DIR)/builtins
CLEANER_DIR = $(SRC_DIR)/cleaner
CONFIG_DIR = $(SRC_DIR)/config
ENV_DIR = $(SRC_DIR)/env
EXEC_DIR = $(SRC_DIR)/exec
EXPANDER_DIR = $(SRC_DIR)/expander
HEREDOC_DIR = $(SRC_DIR)/heredoc
MAIN_DIR = $(SRC_DIR)/main
PARSER_DIR = $(SRC_DIR)/parser
PIPE_DIR = $(SRC_DIR)/pipe
REDIRECTS_DIR = $(SRC_DIR)/redirects
SPLITTER_DIR = $(SRC_DIR)/splitters
PROMPT_DIR = $(SRC_DIR)/start
UTILS_DIR = $(SRC_DIR)/utilsg

#Header file
HEADER = include/minishell.h

# Fontes
SRCS = $(MAIN_DIR)/main.c \
	$(PROMPT_DIR)/prompt.c $(PROMPT_DIR)/prompt_start.c \
	$(PROMPT_DIR)/input_check_quotes.c $(PROMPT_DIR)/input_check_sintax.c \
	$(ENV_DIR)/envp.c $(ENV_DIR)/envp_utils.c \
	$(UTILS_DIR)/utils_general.c $(UTILS_DIR)/utils_path.c $(UTILS_DIR)/utils_string.c \
	$(BUILTINS_DIR)/echo.c $(BUILTINS_DIR)/cd.c $(BUILTINS_DIR)/builtins.c $(BUILTINS_DIR)/builtins_utils.c \
	$(CONFIG_DIR)/path.c $(CONFIG_DIR)/what_type.c $(CONFIG_DIR)/quotes_utils.c \
	$(SPLITTER_DIR)/splitter.c $(SPLITTER_DIR)/splitter_utils.c \
	$(SPLITTER_DIR)/delim_split.c $(SPLITTER_DIR)/delim_split_2.c \
	$(SPLITTER_DIR)/the_split.c $(SPLITTER_DIR)/split_utils.c \
	$(HEREDOC_DIR)/heredoc.c $(HEREDOC_DIR)/heredoc_utils.c $(HEREDOC_DIR)/heredoc_utils_2.c \
	$(REDIRECTS_DIR)/redirects.c $(REDIRECTS_DIR)/redirects_utils.c $(REDIRECTS_DIR)/redirects_in.c $(REDIRECTS_DIR)/redirects_out.c \
	$(PARSER_DIR)/lexer.c $(PARSER_DIR)/parser.c \
	$(PARSER_DIR)/parser_nodes.c $(PARSER_DIR)/parser_nodes_utils.c $(PARSER_DIR)/parser_validations.c \
	$(EXPANDER_DIR)/expander.c $(EXPANDER_DIR)/expander_checkers.c $(EXPANDER_DIR)/expander_validations.c \
	$(EXPANDER_DIR)/expander_envp.c $(EXPANDER_DIR)/expander_status.c \
	$(EXEC_DIR)/exec.c $(EXEC_DIR)/exec_utils.c $(EXEC_DIR)/exec_utils_2.c $(EXEC_DIR)/exec_helper.c \
	$(PIPE_DIR)/pipe.c $(PIPE_DIR)/pipe_utils.c \
	$(CLEANER_DIR)/to_exit.c $(CLEANER_DIR)/to_free.c \

	 
SRC = $(shell find $(SRC_DIR) -type f -name '*.c')
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

#Flags and complilation
RM = rm -f
FLAGS = -g -Wall -Wextra -Werror
READLINE = -lreadline

#Rules
all: $(NAME)

$(NAME): $(OBJS)
	cc $(FLAGS) $(OBJS) -o $(NAME) $(READLINE)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADER)
	@mkdir -p $(OBJ_DIR) $(dir $@)
	$(CC) $(FLAGS) -c $< -o $@

#Cleaning rules
clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(OBJ_DIR) $(NAME)

re: fclean all

#Debugging with valgrind
hell: re
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=rl.supp ./minishell

.PHONY: all clean fclean re
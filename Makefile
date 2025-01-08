# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rsaueia- <rsaueia-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/04 15:04:46 by rsaueia-          #+#    #+#              #
#    Updated: 2025/01/07 21:04:14 by rsaueia-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

HEADER = minishell.h

SRCS = main.c \
	   prompt.c \
	   prompt_start.c \
	   envp.c \
	   envp_utils.c \
	   utils_general.c \
	   utils_path.c \
	   utils_string.c \
	   builtins.c \
	   builtins_utils.c \
	   cd.c \
	   echo.c \
	   path.c \
	   splitter.c \
	   splitter_utils.c \
	   delim_split.c \
	   delim_split_2.c \
	   input_check.c \
	   input_check_2.c \
	   split_utils.c \
	   exec.c \
	   exec_2.c \
	   exec_utils.c \
	   exec_utils_2.c \
	   exec_utils_3.c \
	   exec_helper.c \
	   lexer.c \
	   libft.c \
	   libft_join.c \
	   libft_memory.c \
	   libft_string.c \
	   to_exit.c \
	   to_free_lists.c \
	   to_free.c \
	   the_split.c \
	   parser.c \
	   parser_nodes.c \
	   parser_nodes_utils.c \
	   parser_validations.c \
	   quotes_utils.c \
	   expander.c \
	   expander_checkers.c \
	   expander_checkers_2.c \
	   expander_envp.c \
	   expander_status.c \
	   expander_validations.c \
	   what_type.c \
	   heredoc.c \
	   heredoc_utils.c \
	   heredoc_utils_2.c \
	   redirects.c \
	   redirects_utils.c \
	   redirects_in.c \
	   redirects_out.c \
	   pipe.c \
	   pipe_utils.c \

OBJS = $(SRCS:.c=.o)

RM = rm -f

FLAGS = -g -Wall -Wextra -Werror

$(NAME): $(NAME)
	ar rcs $(NAME)

all: $(NAME)

$(NAME): $(OBJS)
	cc $(FLAGS) $(OBJS) -o $(NAME) -lreadline
	
%.o: %.c $(HEADER)
	cc $(FLAGS) -c $<

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(OBJS) $(NAME)

re: fclean all


hell: re
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --suppressions=rl.supp ./minishell

.PHONY: all clean fclean re
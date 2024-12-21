# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rsaueia- <rsaueia-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/04 15:04:46 by rsaueia-          #+#    #+#              #
#    Updated: 2024/12/21 20:10:16 by rsaueia-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

HEADER = minishell.h

SRCS = main.c \
	   prompt.c \
	   envp.c \
	   envp_utils.c \
	   utils.c \
	   builtins.c \
	   builtins_utils.c \
	   cd.c \
	   echo.c \
	   path.c \
	   splitter.c \
	   delim_split.c \
	   delim_split_2.c \
	   input_check.c \
	   input_check_2.c \
	   split.utils.c \
	   exec.c \
	   exec_utils.c \
	   exec_utils_2.c \
	   exec_helper.c \
	   lexer.c \
	   libft.c \
	   libft_join.c \
	   libft_memory.c \
	   libft_string.c \
	   to_free.c \
	   the_split.c \
	   parser.c \
	   parser_utils.c \
	   expander.c \
	   what_type.c \
	   heredoc.c \
	   heredoc_utils.c \
	   heredoc_utils_2.c \
	   redirects.c \
	   redirects_utils.c \
	   pipe.c \
	   pipe_utils.c \

OBJS = $(SRCS:.c=.o)

RM = rm -f

FLAGS = -Wall -Wextra -Werror

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

.PHONY: all clean fclean re
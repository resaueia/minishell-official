# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rsaueia <rsaueia@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/04 15:04:46 by rsaueia-          #+#    #+#              #
#    Updated: 2024/11/13 18:11:07 by rsaueia          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

HEADER = minishell.h

SRCS = main.c \
	   prompt.c \
	   signals.c \
	   envp.c \
	   utils.c \
	   builtins.c \
	   splitter.c \
	   delim_split.c \
	   input_check.c \
	   split.utils.c \
	   heredoc.c \
	   redirects.c \
	   exec.c \
	   pipe.c \
	   free_functions.c

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
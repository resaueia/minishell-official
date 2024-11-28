# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jparnahy <jparnahy@student.42.rio>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/04 15:04:46 by rsaueia-          #+#    #+#              #
#    Updated: 2024/11/28 17:41:21 by jparnahy         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

HEADER = minishell.h

SRCS = main.c \
	   prompt.c \
	   envp.c \
	   utils.c \
	   builtins.c \
	   splitter.c \
	   delim_split.c \
	   input_check.c \
	   split.utils.c \
	   exec.c \
	   lexer.c \
	   libft.c \
	   to_free.c \
	   the_split.c \
	   parser.c \
	   expander.c \
	   what_type.c \
	   heredoc.c \
	   redirects.c \
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
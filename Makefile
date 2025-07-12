# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yolaidi- <yolaidi-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/20 14:24:27 by yolaidi-          #+#    #+#              #
#    Updated: 2025/06/25 16:21:43 by yolaidi-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
FLAGS = -Wall -Wextra -Werror
RM = rm -rf
FRAMEWORKS = -lreadline -lhistory
OBJS = ${SRCS:.c=.o}
SRCS = main.c \
		parsing/tokenization.c \
		parsing/parsing.c \
		parsing/free_args.c \
		utils/utils1.c \
		utils/utils2.c \
		parsing/copy_env.c \
		parsing/parse_cmd.c \
		parsing/split.c \

.SECONDARY:

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME) $(FRAMEWORKS)

%.o: %.c minishell.h
	$(CC) $(FLAGS) -c $< -o $@

clean:
	${RM} $(OBJS)

fclean: clean
	${RM} $(NAME)

re: fclean all

.PHONY: all clean fclean re
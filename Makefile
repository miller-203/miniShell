# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abdelilah <abdelilah@student.42.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/20 14:24:27 by yolaidi-          #+#    #+#              #
#    Updated: 2025/07/19 14:55:43 by abdelilah        ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
FLAGS = -Wall -Wextra -Werror
RM = rm -rf
FRAMEWORKS = -lreadline -lhistory
OBJS = $(SRCS:.c=.o)
SRCS = main.c parsing/tokenization.c parsing/parsing.c parsing/free_args.c \
	utils/utils1.c utils/utils2.c parsing/copy_env.c parsing/parse_cmd.c parsing/split.c \
	utils/export.c utils/unset.c utils/heredoc.c utils/helper.c \
	exec/builtins_cd.c exec/builtins_pwd.c exec/builtins_echo.c exec/builtins_env.c exec/builtins_exit.c exec/builtins_export.c exec/builtins_unset.c \
	exec/env_list_to_environ.c exec/free_environ.c exec/expand_vars.c exec/execute_command.c exec/execute_ast.c \
	exec/apply_redirections.c exec/find_in_path.c exec/expand_utils.c exec/execute_utils.c

.SECONDARY:

all: $(NAME)
	@$(RM) $(OBJS)

$(NAME): $(OBJS)
	@$(CC) $(FLAGS) $(OBJS) -o $(NAME) $(FRAMEWORKS)

%.o: %.c minishell.h
	@$(CC) $(FLAGS) -c $< -o $@

clean:
	@${RM} $(OBJS)

fclean: clean
	@${RM} $(NAME)

re: fclean all

.PHONY: all clean fclean re

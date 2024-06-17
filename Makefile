# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abolea <abolea@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/12 14:27:40 by abolea            #+#    #+#              #
#    Updated: 2024/06/17 15:23:08 by abolea           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc

FLAGS = -Wall -Werror -Wextra -fsanitize=thread

NAME = philo

SRCS =  main.c \
		init.c \
		routine.c \
		utils.c

OBJ = $(patsubst %.c, .obj/%.o, $(SRCS))

all: $(NAME)

.obj/%.o: %.c philo.h | .obj
	$(CC) $(FLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME)

.obj:
	mkdir -p .obj

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)
	rm -rf .obj

re: fclean all

.PHONY: all clean cleanf re re .obj
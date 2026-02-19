# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jriga <jriga@student.s19.be>               +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/16 16:39:26 by jriga             #+#    #+#              #
#    Updated: 2026/02/19 19:35:35 by jriga            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= philo

CC		= cc
CFLAGS		= -Wall -Wextra -Werror -pthread -g
DEBUG_FLAGS	= -Wall -Wextra -Werror -pthread -g -fsanitize=address,undefined

SRCS_DIR	= srcs
OBJS_DIR	= objs
INCS_DIR	= includes

SRCS	= philo.c utils.c debug.c mutex.c routine.c monitor.c

OBJS	= $(addprefix $(OBJS_DIR)/, $(SRCS:.c=.o))

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(OBJS_DIR)
	$(CC) $(CFLAGS) -I$(INCS_DIR) -c $< -o $@

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

debug: fclean
	$(MAKE) CFLAGS="$(DEBUG_FLAGS)"

.PHONY: all clean fclean re debug

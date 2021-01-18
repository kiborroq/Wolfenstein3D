# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kiborroq <kiborroq@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/10/31 11:14:47 by kiborroq          #+#    #+#              #
#    Updated: 2020/11/02 16:57:10 by kiborroq         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= cub3D

FT			= ft

LIBFT_DIR 	= libft/
SRCS_DIR	= srcs/
INCL_DIR	= incs/

SRCS		= ${SRCS_DIR}main.c \
			${SRCS_DIR}init.c \
			${SRCS_DIR}init_utils.c \
			${SRCS_DIR}parser.c \
			${SRCS_DIR}parser_utils.c \
			${SRCS_DIR}map_parser.c \
			${SRCS_DIR}map_parser_utils.c \
			${SRCS_DIR}config_parser.c \
			${SRCS_DIR}config_parser_utils.c \
			${SRCS_DIR}save.c \
			${SRCS_DIR}hook_handle.c \
			${SRCS_DIR}hook_handle_utils.c \
			${SRCS_DIR}raycast.c \
			${SRCS_DIR}draw_wall_ceil_floor.c \
			${SRCS_DIR}draw_sprites.c \
			${SRCS_DIR}draw_utils.c \
			${SRCS_DIR}close.c \
			${SRCS_DIR}get_next_line.c

OBJS		= ${SRCS:.c=.o}

CC			= gcc
CFLAGS		= -Werror -Wextra -Wall -g

LIBS		= -L ${LIBFT_DIR} -l${FT} -lmlx -lm -lX11 -lbsd -lXext

.c.o :
			@${CC} ${CFLAGS} -c $< -o ${<:.c=.o} -I ${INCL_DIR}

${NAME} : 	${OBJS}
			@make bonus -C ${LIBFT_DIR}
			@echo Create ${NAME}
			@${CC} ${CFLAGS} ${OBJS} ${LIBS} -o ${NAME}

all :		${NAME}

clean :
			@make clean -C ${LIBFT_DIR}
			@rm -f ${OBJS}

fclean : 	clean
			@make fclean -C ${LIBFT_DIR}
			@echo Remove ${NAME}
			@rm -f ${NAME}

re : 		fclean all

.PHONY: 	all clean fclean re .c.o test

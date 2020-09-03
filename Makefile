NAME = webserv

CC = clang++

CFLAGS = -g -Wall -Wextra -Werror

SRCS =  Server.cpp

OBJS = $(SRCS:.c=.o)

LIB_FT = libft/libft.a #-L./libft -lft 로 해도 같다!

RM = rm -f


$(NAME) : ${OBJS} ${LIB_FT}
		${CC} ${CFLAGS} ${OBJS} ${LIB_FT} -o $(NAME)

%.o : %.c
		${CC} ${CFLAGS} -c $< -o $@

${LIB_FT} :
		  make -C ./libft/

all :	${NAME}

clean:
		${RM} ${OBJS}
		make -C ./libft clean 

fclean:	clean
		${RM} ${NAME}
		make -C ./libft fclean

re : fclean all

.PHONY : all re clean fclean

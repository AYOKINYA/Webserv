NAME = webserv

CC = clang++

CFLAGS = -g -Wall -Wextra -Werror

SRCS =  Server.cpp

OBJS = $(SRCS:.cpp=.o)

LIB_FT = libft/libft.a #-L./libft -lft 로 해도 같다!

RM = rm -f

all :	${NAME}

$(NAME) : ${OBJS} ${LIB_FT}
		${CC} ${CFLAGS} ${OBJS} ${LIB_FT} -o $(NAME)

%.o : %.cpp
		${CC} ${CFLAGS} -c $< -o $@


${LIB_FT} :
		  make -C ./libft/

clean:
		${RM} ${OBJS}
		make -C ./libft clean 

fclean:	clean
		${RM} ${NAME}
		make -C ./libft fclean

re : fclean all

.PHONY : all re clean fclean

NAME        = webserve
CXX         = c++
CXXFLAGS     = #-Wall -Wextra -Werror #-std=c++98
DEBUG       = -g
INC         =	./parse/ServerManager.hpp \
                ./parse/ServerConfig.hpp \
				./parse/LocationConfig.hpp

SRCS        =	./parse/ServerConfig.cpp \
				./parse/LocationConfig.cpp \
				main.cpp
# ./parse/ServerManager.cpp
OBJS        = $(SRCS:.cpp=.o)
RM          = rm -f

.cpp.o:
	${CXX} ${CXXFLAGS} ${DEBUG} -c $< -o ${<:.cpp=.o}

$(NAME): ${OBJS}
	${CXX} ${CXXFLAGS} ${DEBUG} $(OBJS) -I server -o $(NAME)

all: $(NAME)

fclean: clean
	$(RM) $(NAME)

clean:
	$(RM) $(OBJS)

re: fclean all
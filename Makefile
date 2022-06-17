NAME        = webserv

CXX         = c++
CXXFLAGS    = -Wall -Wextra -Werror -std=c++98
DEBUG       = -g #-D NDEBUG
LOGLEVEL    = -DLOG_LEVEL=5

INC         =	-I .

SRCS        =	VirtualServerConfig.cpp \
				Log.cpp \
				Request.cpp \
				Response.cpp \
				LocationConfig.cpp \
				Location.cpp \
				VirtualServer.cpp \
				FTServer.cpp \
				Connection.cpp \
				EventHandler.cpp \
				EventContext.cpp \
				main.cpp

OBJS        = $(SRCS:.cpp=.o)
RM          = rm -f

.cpp.o:
				${CXX} ${CXXFLAGS} ${DEBUG} ${LOGLEVEL} -c $< -o ${<:.cpp=.o}

$(NAME): ${OBJS}
				${CXX} ${CXXFLAGS} ${DEBUG} $(OBJS) $(INC) -o $(NAME)

all: $(NAME)

fclean: clean
				$(RM) $(NAME)

clean:
				$(RM) $(OBJS)

re: fclean all

NAME = ircserver

CXX = c++
# CXXFLAGS = -Wall -Werror -Wextra -std=c++98  -fsanitize=address -g3
CXXFLAGS = -Wall -Werror -Wextra -std=c++98  

OBJDIR = ./obj/
SRCDIR = ./src/

SRC = Server.cpp Client.cpp parsing.cpp main.cpp commands.cpp Channel.cpp serverChannel.cpp
# SRC = Server.cpp Client.cpp parsing.cpp commandPath.cpp Channel.cpp serverChannel.cpp

OBJ = $(addprefix $(OBJDIR), $(SRC:.cpp=.o))

all:	$(OBJDIR) $(NAME) 

$(OBJDIR):
	@if [ ! -d "$(OBJDIR)" ]; then mkdir -p $(OBJDIR); fi

$(NAME):	$(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

$(OBJDIR)%.o:	$(SRCDIR)%.cpp	
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	@ rm -r $(OBJDIR)/*
	@ rm $(NAME)

fclean:	
	@ rm -rf $(OBJDIR)
	@ rm -f $(NAME)

re: fclean all

server: 
	@ ./$(NAME) 4242 asdf

	
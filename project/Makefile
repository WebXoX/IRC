NAME = ircserver

CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98 -g

OBJDIR = ./obj/
SRCDIR = ./src/

SRC = server_t.cpp parsing.cpp
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
	@ ./$(NAME)

	
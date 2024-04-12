NAME = ircserver

CXX = c++
# CXXFLAGS = -Wall -Werror -Wextra -std=c++98  -fsanitize=address -g3
CXXFLAGS = -Wall -Werror -Wextra -std=c++98  

RED = \033[1;31m
GREEN = \033[1;32m
YELLOW = \033[1;33m
BLUE = \033[1;34m
RESET = \033[0m

OBJDIR = ./obj/
SRCDIR = ./src/

SRC = Server.cpp Client.cpp parsing.cpp main.cpp CommandCalls.cpp Channel.cpp CmdKick.cpp CmdTopic.cpp CmdJoin.cpp CmdInvite.cpp CmdPrivmsg.cpp utils.cpp
# SRC = Server.cpp Client.cpp parsing.cpp commandPath.cpp Channel.cpp serverChannel.cpp

OBJ = $(addprefix $(OBJDIR), $(SRC:.cpp=.o))

all:	$(OBJDIR) $(NAME) 

$(OBJDIR):
	@if [ ! -d "$(OBJDIR)" ]; then mkdir -p $(OBJDIR); fi

$(NAME):	$(OBJ)
	@$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

$(OBJDIR)%.o:	$(SRCDIR)%.cpp	
	@printf "$(YELLOW)Compiling$(RESET) $<:\r\t\t\t\t\t"
	@$(CXX) $(CXXFLAGS) -c -o $@ $<
	@printf "\r\t\t\t\t\t$(RED)$(CXXFLAGS)$(RESET) $(GREEN)<DONE>$(RESET)\n"

clean:
	@ rm -r $(OBJDIR)/*
	@ rm $(NAME)

fclean:	
	@ rm -rf $(OBJDIR)
	@ rm -f $(NAME)

re: fclean all

server: 
	@ ./$(NAME) 4242 1234

	
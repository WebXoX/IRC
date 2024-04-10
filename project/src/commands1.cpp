#include "../inc/Server.hpp"
#include "../inc/Reply.hpp"
#include <poll.h>
#include <stack>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <string.h>


int Server::MOTD(Client * user)
{
    this->definedmessage(user->client_fd, RPL_MOTDSTART(user->hostname,this->server_name));
    std::srand(static_cast<unsigned>(time(0)));
    int num = (rand() % 7 ) + 1;
    std::ostringstream oss;
    oss << num;
    std::string line;
    std::ifstream infile;
    infile.open(("./src/messages/motd"+  oss.str() + ".txt").c_str(), std::ios::in);
    if ( infile.is_open())
    {
        while (std::getline(infile,line))
        {
            this->definedmessage(user->client_fd, RPL_MOTD(this->server_name,line));
        }
        infile.close();
    }
    else
        this->definedmessage(user->client_fd, ERR_NOMOTD(this->server_name));
    this->definedmessage(user->client_fd, RPL_ENDOFMOTD(this->server_name));
    return 1;
}
std::string int_tostring(int guest)
{
    std::string str;
    std::ostringstream oss;
    oss << guest;
    str = oss.str();
    return str;
}
void Server::adduser(Client * user, ircMessage msg)
{
    if(user->username.empty() == true)
    {
        if(user->username.empty() == true)
        {
            user->username = msg.params[0];
            if(msg.params.size() > 2)
            {
                if(msg.params[1].compare("0") && msg.params[2].compare("*") && msg.trailing.empty() == false)
                    user->realname = msg.trailing;
                else
                    user->realname = msg.params[1];
			}
            user->regi_status = 6;
            this->definedmessage(user->client_fd, RPL_WELCOME(user_id(user->nickname,user->username),user->nickname));
            this->definedmessage(user->client_fd, RPL_YOURHOST(user->hostname,"irssi", "1"));
            this->definedmessage(user->client_fd, RPL_CREATED(user->hostname,this->creation_date));
            this->definedmessage(user->client_fd, RPL_MYINFO(user->hostname,"irssi", "1", "","",""));
            this->definedmessage(user->client_fd, RPL_ISUPPORT(user->hostname,""));
            MOTD(user);
        }
        while(user->nick_status == 1)
        {
            std::string str = "guest";
            str += int_tostring(this->guestuser);
            nick(user,str);
            std::cout << "nicks " << str << std::endl;
            this->guestuser++;
        }
    }
}
void Server::nick(Client * user, std::string str)
{
    if(this->nicknames.empty() == false && std::find(this->nicknames.begin(), this->nicknames.end(), str) != this->nicknames.end())
    {
            definedmessage(user->client_fd ,ERR_NICKNAMEINUSE(this->server_name,str));
            user->nick_status = 1;
    }
    else if(str.find_first_of("# @:&") != std::string::npos)
    {
        user->nick_status = 1;
        definedmessage(user->client_fd ,ERR_ERRONEUSNICKNAME(this->server_name,str));
    }
    else if (str.empty() == true)
    {
        user->nick_status = 1;
        definedmessage(user->client_fd,ERR_NONICKNAMEGIVEN(this->server_name));
    }
    else
    {
        user->nick_status = 0;
        this->nicknames.push_back(str);
        user->regi_status = 5;
            std::cout << "------------------" << std::endl;

        for (size_t i = 0; i < this->nicknames.size(); i++)
        {
            std::cout << "nicks " << this->nicknames[i] << std::endl;
        }
            std::cout << "------------------" << std::endl;
        
        if(user->nickname.empty() == false)
        {

            this->nicknames.erase(std::find(this->nicknames.begin(), this->nicknames.end(), user->nickname));
            this->definedmessage(user->client_fd, RPL_NICK(this->server_name,user->nickname,str));
            // str1 = this->msg("irssi", NULL,user->nickname,"NICK "+ str).c_str();
            // len = str1.length();
            // send(user->client_fd,str1.c_str(),len,0);
            user->nickname.clear();
        }
        user->nickname = str;
    }
}
void Server::cap_ls(Client *user)
{
    std::string str =  this->msg("irssi", "CAP * LS", "","").c_str();
    int len = str.length();
    user->regi_status = 1;
    send(user->client_fd,str.c_str(),len,0);
}

void Server::cap_ack(Client *user)
{
    std::string str = this->msg("irssi", "CAP * ACK","","").c_str();
    int len = str.length();
    user->regi_status = 2;
    send(user->client_fd,str.c_str(),len,0);
}

int Server::register_user(ircMessage msg, Client * user)
{
    if(msg.command.compare("CAP") == 0)
    {
        if(msg.params[0].compare("LS") == 0 && user->regi_status == 0)
            cap_ls(user);
        else if(msg.params[0].compare("REQ") == 0 && user->regi_status == 1)
            cap_ack(user);
        else if(msg.params[0].compare("END") == 0 && user->regi_status == 2)
        {
            user->regi_status = 3;
        }
        else
            definedmessage(user->client_fd ,ERR_ALREADYREGISTERED(this->server_name));
        return 1;
    }
    else if(msg.command.compare("PASS") == 0)
    {
        if(user->pass_status == 0)
        {
            if(msg.params[0].compare(this->pass) == 0)
            {
                user->regi_status = 4;
                user->pass_status = 1;
            }
            else
            {
                close(user->client_fd);
                user->client_fd = -1;
                definedmessage(user->client_fd ,ERR_PASSWDMISMATCH(this->server_name));
            }
        }
        else
            definedmessage(user->client_fd ,ERR_ALREADYREGISTERED(this->server_name));
        return 1;    
    }	
    else if(msg.command.compare("NICK") == 0 )
    {
        nick(user,msg.params[0]);
        return 1;    
    }
    else if(msg.command.compare("USER") == 0 && (user->regi_status == 4 ||user->regi_status == 5))
    {
        adduser(user,msg);
        return 1;    
    }
    return 0;
    // else if ( user->regi_status == 6)
    //     definedmessage(user->client_fd ,ERR_ALREADYREGISTERED(this->server_name));
}


void Server::commandPath(ircMessage msg, Client * user)
{
	std::string		str;
    if(msg.params.size() > 0)
    {
        if(register_user(msg,user) == 1)
        {

        }
		else if(msg.command.compare("PING") == 0)
		{	
            if(user->regi_status > 1)
                this->definedmessage(user->client_fd,RPL_PONG(user_id(user->nickname,user->username),msg.params[0]));
        }
        else if (msg.command.compare("JOIN") == 0)
            this->joinCommand(msg, *user);
        else
			std::cerr << "Invalid command" << std::endl;
	}
    else if(msg.command.compare("MOTD") == 0 && user->regi_status == 6)
        MOTD(user);
	else
        this->definedmessage(user->client_fd,ERR_NEEDMOREPARAMS(this->server_name ,msg.command));
	str.clear();
}
	
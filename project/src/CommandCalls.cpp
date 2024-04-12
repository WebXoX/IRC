#include "../inc/Server.hpp"
#include "../inc/Reply.hpp"
#include <poll.h>
#include <stack>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <string.h>

void Server::callMotd(Client * user)
{
    if(user->registerstatus() == true)
    {
        this->definedmessage(user->client_fd, RPL_WELCOME(user_id(user->nickname,user->username),user->nickname));
        this->definedmessage(user->client_fd, RPL_YOURHOST(user->hostname,"irssi", "1"));
        this->definedmessage(user->client_fd, RPL_CREATED(user->hostname,this->creation_date));
        this->definedmessage(user->client_fd, RPL_MYINFO(user->hostname,"irssi", "1", "","",""));
        this->definedmessage(user->client_fd, RPL_ISUPPORT(user->hostname,""));
        MOTD(user);
    }
}
int Server::MOTD(Client * user)
{
    // (void)user;
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
            callMotd(user);
        }
        // while(user->nick_status == 1)
        // {
        //     std::string str = "guest";
        //     str += int_tostring(this->guestuser);
        //     nick(user,str);
        //     // std::cout << "nicks " << str << std::endl;
        //     this->guestuser++;
        // }
    }
}
void Server::nick(Client * user, std::string str)
{
    if(this->nicknames.empty() == false && std::find(this->nicknames.begin(), this->nicknames.end(), str) != this->nicknames.end())
    {
            definedmessage(user->client_fd ,ERR_NICKNAMEINUSE(this->server_name,str));
    }
    else if(str.find_first_of("# @:&") != std::string::npos)
    {
        definedmessage(user->client_fd ,ERR_ERRONEUSNICKNAME(this->server_name,str));
    }
    else if (str.empty() == true)
    {
        definedmessage(user->client_fd,ERR_NONICKNAMEGIVEN(this->server_name));
    }
    else
    {
        this->nicknames.push_back(str);
            std::cout << "------------------" << std::endl;
            for (size_t i = 0; i < this->nicknames.size(); i++)
                std::cout << "nicks " << this->nicknames[i] << std::endl;
            std::cout << "------------------" << std::endl;
        if(user->nickname.empty() == false)
        {
            for(std::vector<std::string>::iterator it = this->nicknames.begin(); it != this->nicknames.end(); ++it)
            {
                if(*it == user->nickname)
                {
                    this->nicknames.erase(it);
                    break;
                }
            }
            this->definedmessage(user->client_fd, RPL_NICK(user->nickname,user->username,str));
            user->nickname.clear();
        }
        user->nickname = str;
    }
}
void Server::cap_ls(Client *user)
{
    std::string str =  this->msg("irssi", "CAP * LS", "","").c_str();
    int len = str.length();
    send(user->client_fd,str.c_str(),len,0);
}

void Server::cap_ack(Client *user)
{
    std::string str = this->msg("irssi", "CAP * ACK","","").c_str();
    int len = str.length();
    send(user->client_fd,str.c_str(),len,0);
}

int Server::register_user(ircMessage msg, Client * user)
{
    if(user->registerstatus() == false)
    {
        if(msg.command.compare("CAP") == 0)
        {
            if(msg.params[0].compare("LS") == 0)
                cap_ls(user);
            else if(msg.params[0].compare("REQ") == 0)
                cap_ack(user);
            else if(msg.params[0].compare("END") == 0)
            {
                // user->regi_status = 3;
            }
            return 1;
        }
        else if(msg.command.compare("PASS") == 0)
        {
            if(user->pass_status == 0)
            {
                if(msg.params[0].compare(this->pass) == 0)
                {
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
        else if(msg.command.compare("USER") == 0 )
        {
            adduser(user,msg);
            return 1;    
        }
    }
    
    return 0;
}


void Server::commandPath(ircMessage msg, Client * user)
{
	std::string		str;
    printCommand(msg);
    std::map<std::string, Channel>::iterator it;
    for (it = this->channels.begin(); it != this->channels.end(); it++)
    {
        std::cout << "channel name: " << it->first << std::endl;
    }
    if(msg.params.size() > 0)
    {
        if(user->registerstatus()== false) 
        {
            register_user(msg,user);
        }
        else 
        {
            if(msg.command.compare("PING") == 0)
            {	
                    this->definedmessage(user->client_fd,RPL_PONG(user_id(user->nickname,user->username),msg.params[0]));
            }
            else if (msg.command.compare("JOIN") == 0)
            {
                this->joinCommand(msg, *user);
            }
            else if (msg.command.compare("TOPIC") == 0) {
                this->topicCommand(msg, *user);
            }
            else if (msg.command.compare("KICK") == 0) {
                this->kickCommand(msg, *user);
            }
            else if (msg.command.compare("PRIVMSG") == 0) {
                this->privmsgCommand(msg, *user);
            }
            else if (msg.command.compare("INVITE") == 0) {
                this->inviteCommand(msg, *user);
            }
            else if(msg.command.compare("MOTD") == 0)
                MOTD(user);
            else 
            {
                std::string message = "<" + user->nickname + "> " + msg.message;
                this->channels[user->currentChannel].broadcast(message);
            }
        }
        if(msg.command.compare("NICK") == 0 )
        {
            if(user->registerstatus() == true)
            {
                nick(user,msg.params[0]);
            }
            else
            {
                nick(user,msg.params[0]);
                callMotd(user);
            }
        }
	}
	else
        this->definedmessage(user->client_fd,ERR_NEEDMOREPARAMS(this->server_name ,msg.command));
	str.clear();
}
	
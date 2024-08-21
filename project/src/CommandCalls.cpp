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
        user->toSend.push_back( RPL_WELCOME(user_id(user->nickname,user->username),user->nickname));
        user->toSend.push_back( RPL_YOURHOST(user->username,"irssi", "1"));
        user->toSend.push_back( RPL_CREATED(user->username,this->creation_date));
        user->toSend.push_back( RPL_MYINFO(user->username,"irssi", "1", "","",""));
        user->toSend.push_back( RPL_ISUPPORT(user->username,"CHANMODES=ikolt"));
        MOTD(user);
    }
}
int Server::MOTD(Client * user)
{
    user->toSend.push_back( RPL_MOTDSTART(user->username,this->server_name));
    std::srand(static_cast<unsigned>(time(0)));
    int num = (rand() % 7 ) + 1;
    std::ostringstream oss;
    oss << num;
    std::string line;
    std::ifstream infile;
    infile.open(("./src/messages/motd"+  oss.str() + ".txt").c_str(), std::ios::in);
    if (infile.is_open())
    {
        while (std::getline(infile,line))
        {
            user->toSend.push_back( RPL_MOTD(this->server_name,line));
        }
        infile.close();
    }
    else
        user->toSend.push_back( ERR_NOMOTD(this->server_name));
    user->toSend.push_back( RPL_ENDOFMOTD(this->server_name));
    return 1;
}
void Server::adduser(Client * user, ircMessage msg)
{
    std::cout << "------------------user -------------" << std::endl;
    if(user->username.empty() == true && msg.params.size() > 0)
    {
        std::cout << "------------------user -------------" << std::endl;
        user->username = msg.params[0];
        if(msg.params.size()  > 1)
        {
            if(msg.params[1].compare("0") && msg.params[2].compare("*") && msg.trailing.empty() == false)
                user->realname = msg.trailing;
            else
                user->realname = msg.params[1];
        }
        else
            user->realname = user->username;
        callMotd(user);
    }
}
void Server::nick(Client * user, std::string str)
{
    if(this->nicknames.empty() == false && std::find(this->nicknames.begin(), this->nicknames.end(), str) != this->nicknames.end())
    {
            user->toSend.push_back(ERR_NICKNAMEINUSE(this->server_name,str));
    }
    else if(str.find_first_of("# @:&") != std::string::npos)
    {
        user->toSend.push_back(ERR_ERRONEUSNICKNAME(this->server_name,str));
    }
    else if (str.empty() == true)
    {
        user->toSend.push_back(ERR_NONICKNAMEGIVEN(this->server_name));
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
            user->toSend.push_back( RPL_NICK(user->nickname,user->username,str));
            user->nickname.clear();
        }
        user->nickname = str;
    }
}
void Server::cap_ls(Client *user)
{
    user->toSend.push_back(this->msg("irssi", "CAP * LS", "",""));
}

void Server::cap_ack(Client *user)
{
    user->toSend.push_back(this->msg("irssi", "CAP * ACK","",""));
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
                    user->toSend.push_back(ERR_PASSWDMISMATCH(this->server_name));
                    // close(user->client_fd);
                    // user->client_fd = -1;
                }
            }
            else
                user->toSend.push_back(ERR_ALREADYREGISTERED(this->server_name));
            return 1;    
        }	
        else if(msg.command.compare("USER") == 0 && user->pass_status == 1 )
        {
            adduser(user,msg);
            return 1;    
        }
    }
    else if (msg.command.compare("USER") == 0 || msg.command.compare("PASS") == 0)
    {
        user->toSend.push_back(ERR_ALREADYREGISTERED(this->server_name));
        return 1;    
    }
    
    return 0;
}
bool Server::validcommand(std::string cmd) {
    std::string cmds[] = {"JOIN", "MODE", "TOPIC", "NICK", "QUIT", "PRIVMSG", "INVITE", "PING", "MOTD","CAP","PASS","USER"};
    for (size_t i = 0; i < 12; ++i) {
        if (cmds[i] == cmd) {
            return true;
        }
    }
    return false;
}
void Server::quitCmd(std::string trailing, Client * user)
{
    for(this->chan_it = this->channels.begin(); this->chan_it != this->channels.end(); this->chan_it++)
    {
        std::string str = " has quit";
        if(this->chan_it->second.isUser(*user) == true)
        {
            this->chan_it->second.removeFromAll(*user);
            if(trailing.empty() == false)
                this->chan_it->second.broadcast_others(*user, RPL_QUIT(user_id(user->nickname,user->username),trailing));
            else
                this->chan_it->second.broadcast_others(*user, RPL_ERROR(user_id(user->nickname,user->username),str));
        
        }
    }
    close(user->client_fd);
    user->client_fd = -1;
}
void Server::commandPath(ircMessage msg, Client * user)
{
	std::string		str;
    printCommand(msg);
    std::map<std::string, Channel>::iterator it;
    std::cout << "---------------channels------------------" << std::endl;
    for (it = this->channels.begin(); it != this->channels.end(); it++)
    {
        std::cout << "channel name: " << it->first << std::endl;
    }
    std::cout << "---------------------------------" << std::endl;

    if(msg.command.compare("QUIT") == 0 )
        quitCmd(msg.trailing,user);
    else if(msg.params.size() > 0 || msg.trailing.empty() == false)
    {
        if(register_user(msg,user) == 1)
        {
        }
        else if (user->registerstatus() == true)
        {
            if(msg.command.compare("PING") == 0)
                user->toSend.push_back(RPL_PONG(user_id(user->nickname,user->username),msg.params[0]));
            else if (msg.command.compare("JOIN") == 0)
                this->joinCommand(msg, *user);
            else if (msg.command.compare("TOPIC") == 0) 
                this->topicCommand(msg, *user);
            else if (msg.command.compare("KICK") == 0) 
                this->kickCommand(msg, *user);
            else if (msg.command.compare("PRIVMSG") == 0) 
                this->privmsgCommand(msg, *user);
            else if (msg.command.compare("INVITE") == 0) 
                this->inviteCommand(msg, *user);
            else if (msg.command.compare("MODE") == 0) 
                this->modeCommand(msg, *user);
            else if(msg.command.compare("MOTD") == 0)
                MOTD(user);
        }
        if(msg.command.compare("NICK") == 0 )
        {
            if(user->registerstatus() == true)
                nick(user,msg.params[0]);
            else if (user->pass_status == 1 && user->registerstatus() == false)
            {
                nick(user,msg.params[0]);
                callMotd(user);
            }
        }
        
	}
	else if (msg.command.empty() == false && validcommand(msg.command)== true)
        user->toSend.push_back(ERR_NEEDMOREPARAMS(this->server_name ,msg.command));
	else if (msg.command.empty() == false && validcommand(msg.command)== false)
        user->toSend.push_back(ERR_UNKNOWNCOMMAND(this->server_name ,msg.command));
	str.clear();
}
	
#include "../inc/Server.hpp"
#include "../inc/Reply.hpp"
#include <poll.h>
#include <stack>
#include <fstream>
#include <iostream>
#include <sys/stat.h>

void cap_ls(Client *user)
{
    std::string str =  this->msg("irssi", "CAP * LS", "","").c_str();
    int len = str.length();
    user->regi_status = 1;
    send(user->client_fd,str.c_str(),len,0);
}

void cap_ack(Client *user)
{
    std::string str = this->msg("irssi", "CAP * ACK","","").c_str();
    int len = str.length();
    user->regi_status = 2;
    send(user->client_fd,str.c_str(),len,0);
}

void Server::register_user(ircMessage msg, Client * user)
{
    if(msg.command.compare("CAP") == 0)
    {
        if(msg.params[0].compare("LS") == 0 && user->regi_status == 0)
            cap_ls();
        else if(msg.params[0].compare("REQ") == 0 && user->regi_status == 1)
            cap_ack();
        else if(msg.params[0].compare("END") == 0 && user->regi_status == 2)
        {
            user->regi_status = 3;
        }
        else
            throw(ERR_ALREADYREGISTERED);
    }
     else if(msg.command.compare("PASS") == 0)
    {
        if(user->regi_status == 3)
        {
            if(msg.params[0].compare(this->pass) == 0)
                user->regi_status = 4;
            else
                throw(ERR_PASSWDMISMATCH);
                // str = this->msg("irssi", "464", msg.params[0], "Password incorrect").c_str();
                // len = str.length();
                // send(user->client_fd,str.c_str(),len,0);
                // close(user->client_fd);
                // user->client_fd = -1;
            
        }
        else
            throw(ERR_ALREADYREGISTERED);
    }	
    else if(msg.command.compare("NICK") == 0 && user->regi_status == 4)
    {
        if(this->nicknames.empty() == false && std::find(this->nicknames.begin(), this->nicknames.end(), msg.params[0]) != this->nicknames.end())
        {
            str = this->msg("irssi", "433", msg.params[0], "Nickname is already in use").c_str();
            len = str.length();
            send(user->client_fd,str.c_str(),len,0);
        }
        else if(msg.params[0].find_first_of("# @:&") != std::string::npos)
        {
            str = this->msg("irssi", "432", msg.params[0], "Erroneous nickname").c_str();
            len = str.length();
            send(user->client_fd,str.c_str(),len,0);
        }
        else if (msg.params[0].empty() == true)
        {
            str = this->msg("irssi", "431", msg.params[0], "No nickname given").c_str();
            len = str.length();
            send(user->client_fd,str.c_str(),len,0);
        }
        else
        {
            this->nicknames.push_back(msg.params[0]);
            user->regi_status = 5;
            if(user->nickname.empty() == false)
            {
                this->nicknames.erase(std::find(this->nicknames.begin(), this->nicknames.end(), user->nickname));
                str = this->msg("irssi", NULL,user->nickname,"NICK "+ msg.params[0]).c_str();
                len = str.length();
                send(user->client_fd,str.c_str(),len,0);
                user->nickname.clear();
            }
            user->nickname = msg.params[0];
        }
        return 1;
    }
    else if(msg.command.compare("USER") == 0 && user->regi_status == 5)
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
            str = this->msg("irssi", "001 "+user->nickname, "Welcome to the IRSSI.Chat Internet Relay Chat Network "+user->username,"").c_str();
            len = str.length();
            send(user->client_fd,str.c_str(),len,0);
			str.clear();
        }
        else
        {
            str = this->msg("irssi", "462", msg.params[0], "You may not reregister").c_str();
            len = str.length();
            send(user->client_fd,str.c_str(),len,0);
        }
    return 1;
        // throw "ERR_ALREADYREGISTERED";
    }
}


void Server::commandPath(ircMessage msg, Client * user)
{
	std::string		str;
	size_t			len;
    if(msg.params.size() > 0)
    {
        if(register_user(msg,user) == 1)
        {
		}
		else if(msg.command.compare("PING") == 0)
		{
			if(user->regi_status == 1)
			{
				str = this->msg("irssi", "PONG", msg.params[0], "").c_str();
				len = str.length();
				send(user->client_fd,str.c_str(),len,0);
			}
		}
        // add other commands here!!!!!!!!!!!!
		else
		{
			std::cerr << "Invalid command" << std::endl;
		}
	}
    else if(msg.command.compare("MOTD") == 0 && user->regi_status == 0)
        {
            str = this->msg("irssi", "375", ":- irssi Message of the Day -", "Message of the Day").c_str();
            len = str.length();
            send(user->client_fd,str.c_str(),len,0);
            std::srand(static_cast<unsigned>(time(0)));
            int num = rand() % 7 + 1;
             num = rand() % 7 + 1;
            std::ostringstream oss;
            oss << num;
            struct stat fileStat;
            std::string line;
	        std::ifstream infile("./src/messages/motd"+  oss.str() + ".txt");
            if ( infile.is_open() && S_ISDIR(fileStat.st_mode) == 0)
            {
                while (std::getline(infile,line))
                {
                    str = this->msg("irssi", "372", ":- ",line ).c_str();
                    len = str.length();
                    send(user->client_fd,str.c_str(),len,0);
                    str.clear();
                }
                infile.close();
            }
            else
                std::cout << "ERROR FILE DOES NOT EXSIST" << std::endl;
            str = this->msg("irssi", "376", "End of /MOTD command.", "Message of the Day").c_str();
            len = str.length();
            send(user->client_fd,str.c_str(),len,0);
        }
	else
	{
		str = this->msg("irssi", "461", msg.command, "Not enough parameters").c_str();
		len = str.length();
		send(user->client_fd,str.c_str(),len,0);
	}
	str.clear();
}
	
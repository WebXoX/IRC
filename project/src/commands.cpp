#include "../inc/Server.hpp"
#include <poll.h>
#include <stack>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
int Server::register_user(ircMessage msg, Client * user)
{
    std::string str;
    int len;
    if(msg.command.compare("CAP") == 0)
    {
        if(msg.params[0].compare("LS") == 0 && user->regi_status == 0)
        {
            std::string ls = cap_ls();
            str = this->msg("irssi", "CAP * LS", ls,"").c_str();
            len = str.length();
            user->regi_status = 1;
            send(user->client_fd,str.c_str(),len,0);
        }
        else if(msg.params[0].compare("REQ") == 0 && user->regi_status == 1)
        {
            // std::cout << msg.params[0] << std::endl;
            str = this->msg("irssi", "CAP * ACK", cap_ack(msg),"").c_str();
            len = str.length();
            user->regi_status = 2;
            send(user->client_fd,str.c_str(),len,0);
        }
        else if(msg.params[0].compare("END") == 0 && user->regi_status == 2)
        {
            if(user->regi_status == 2)
            {
                user->regi_status = 3;
            }
        }
            return 1;
    }
    else if(msg.command.compare("PASS") == 0)
    {
        if(user->regi_status == 3)
        {
            if(msg.params[0].compare(this->pass) == 0)
            {
                user->regi_status = 4;
            }
            else
            {
                str = this->msg("irssi", "464", msg.params[0], "Password incorrect").c_str();
                len = str.length();
                send(user->client_fd,str.c_str(),len,0);
                close(user->client_fd);
                user->client_fd = -1;
            }
            
        }
        else
        {
            str = this->msg("irssi", "462", msg.params[0], "You may not reregister").c_str();
            len = str.length();
            send(user->client_fd,str.c_str(),len,0);
        }
            return 1;
                    // throw  "No re-registeration";
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
        user->nickname = msg.params[1];
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
            str = this->msg("irssi", "001 user", "Welcome to the IRSSI.Chat Internet Relay Chat Network user","").c_str();
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
    return 0;
}
void Server::commandPath(ircMessage msg, Client * user)
{
	std::string		str;
	size_t			len;
    // std::cout << msg.command << std::endl;
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
        else if(msg.command.compare("MOTD") == 0 && user->regi_status == 6)
        {
            str = this->msg("irssi", "375", ":- irssi Message of the Day -", "Message of the Day").c_str();
            len = str.length();
            send(user->client_fd,str.c_str(),len,0);
            srand(time(0));
            int random = rand() % 7 + 1;
            struct stat fileStat;
            std::string line;
	        std::ifstream infile("./messages/motd"+std::to_string(random));
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
        // add other commands here!!!!!!!!!!!!
		else
		{
			std::cerr << "Invalid command" << std::endl;
		}
	}
	else
	{
		str = this->msg("irssi", "461", msg.command, "Not enough parameters").c_str();
		len = str.length();
		send(user->client_fd,str.c_str(),len,0);
	}
	str.clear();
}
	
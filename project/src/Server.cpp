#include "../inc/Server.hpp"
#include <poll.h>
#include <stack>

/* orth Server / constructor*/
Server::Server ()
{
	std::cout << "Server default constructor" << std::endl;
}

Server::Server (std::string port , std::string pass)
{
	try
	{
		this->port = std::atoi(port.c_str());
		this->ports = port;
		this->pass = pass;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
		// g_signal = 0;
	this->multi_prefix = true;
	this->extended_join = true;
	this->away_notify = true;
	this->chghost = true;
	this->account_notify = true;
	this->server_time = true;
	this->sasl = "PLAIN";
	std::cout << "Server default constructor" << std::endl;
}
Server::Server (const Server &a)
{
	std::cout << "Server copy constructor " << std::endl;
	*this = a;
}

Server::~Server ()
{
	std::cout << "Server distructor called" << std::endl;
}

Server& Server::operator=(const Server& rhs)
{
    if (this != &rhs)
    {
        std::cout << "Server copy assignment operator = " << std::endl;
		*this = rhs;
    }
    return *this;
}
/* orth Server */
/*extra*/
 void	Server::sighandle(int sig)
{
	if (sig == SIGINT)
	{
		// g_signal = 1;
		exit(0);
	}
	else if (sig == SIGTSTP)
	{
		// g_signal = 1;
		exit(0);
		
	}
}
int Server::serverInit()
{
    // initization of server socket port poll 
    this->server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fcntl(this->server, F_SETFL, O_NONBLOCK) == -1)
    {
        std::cerr << "Error setting socket to non-blocking" << std::endl;
        return 1;
    }
	if(this->server == -1)
	{
		std::cerr << "Error creating socket" << std::endl;
		return 1;
	}
    //socket config
    this->service.sin_family = AF_INET;
    this->service.sin_addr.s_addr = INADDR_ANY;
    service.sin_port = htons(this->port);
    // poll config
	// memset(&(this->fd_poll), 0, sizeof(this->fd_poll));
	pollfd poll;
	poll.fd = this->server;
	poll.events = POLLIN;
	this->fd_poll.push_back(poll);
    number_of_clients = 1;
	return 0;
}
int Server::runServer()
{
	int optval = 1;
    if (setsockopt(this->server, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
        return -1;
    // binds the socket to the port
	if(bind(this->server, (sockaddr*)&this->service, sizeof(this->service)) == -1)
    {
        std::cerr << "Error binding socket" << std::endl;
        close(this->server);
        return 1;
    }
    // listen for incoming connections
    if(listen(this->server, 1) == -1)
    {
        std::cerr << "Error listening on socket" << std::endl;
        close(this->server);
        return 1;
    }
    return 0;
}

int Server::connectionEvent()
{
    // checking for new connections events
    // signal(SIGTSTP,  this->sighandle);
    // signal(SIGINT, this->sighandle);
    if (this->fd_poll[0].revents & POLLIN)
    {
        Client *new_client = new Client();
        new_client->client_fd = accept(this->server, NULL, NULL);
        if(new_client->client_fd == -1)
        {
            std::cerr << "Error accepting client" << std::endl;
            return 1;
        }
        if (fcntl(new_client->client_fd, F_SETFL, O_NONBLOCK) == -1)
        {
            std::cerr << "Error setting client socket to non-blocking" << std::endl;
            return 1;
        }
        this->client.push_back(new_client);
        std::cerr << " accepting client" << std::endl;
        // client registration 
        // setting poll for tracking events of the new client
		pollfd poll;
		poll.fd = new_client->client_fd;
		poll.events = POLLIN | POLLOUT;
		this->fd_poll.push_back(poll);
        this->number_of_clients++;
    }
    return 0;
}
std::string Server::msg(std::string source, std::string command, std::string param, std::string text)
{
    return (":"+source+ " " +command+" :"+param+" "+text + "\r\n");
}

std::string Server::cap_ls()
{
	std::string cap_list;
	if(this->account_notify)
		cap_list += "account-notify ";
	if(this->away_notify)
		cap_list += "away-notify ";
	if(this->chghost)
		cap_list += "chghost ";
	if(this->extended_join)
		cap_list += "extended-join ";
    if(this->multi_prefix)
		cap_list += "multi-prefix ";
    cap_list += "sasl="+this->sasl+" server-time";
    // return ("account-notify away-notify chghost extended-join multi-prefix sasl=PLAIN server-time");
	return cap_list;
}
std::string Server::cap_ack( ircMessage msg)
{
	std::string str;
	std::string ack;
	std::string cap_list[] = {"account-notify","away-notify","chghost","extended-join","multi-prefix","sasl="+this->sasl+" server-time"};
	// std::cout << msg.message << std::endl;
	// std::cout << msg.trailing << std::endl;
	ack = msg.trailing.substr(0, msg.trailing.find_first_of(" "));
	while (msg.trailing.empty() == false)
	{
		if(std::find(&cap_list[0], (&cap_list[6]), ack) != &cap_list[6])
		{
			str += ack + " ";
		}
		if(msg.trailing.empty() == true)
			break;
        // std::cout << msg.trailing << std::endl;
		msg.trailing.erase(0, msg.trailing.find_first_of(" ") + 1);
        int subint = msg.trailing.find_first_of(" ");
        if(  msg.trailing.find_first_of(" ") == std::string::npos)
        {
            ack = msg.trailing;
            msg.trailing.clear();
        }
        else
            ack = msg.trailing.substr(0, subint);
	}
    return (str);
}
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
            // if(user->regi_status == 1)
            // {
                str = this->msg("irssi", "001 user", "Welcome to the IRSSI.Chat Internet Relay Chat Network user","").c_str();
                len = str.length();
                user->regi_status = 3;
                send(user->client_fd,str.c_str(),len,0);
            // }
            // else
            // {
            //     str = this->msg("irssi", "464", msg.params[0], "Password incorrect").c_str();
            //     len = str.length();
            //     send(user->client_fd,str.c_str(),len,0);
            //     close(user->client_fd);
            //     user->client_fd = -1;
            // }
        }
    }
    else if(msg.command.compare("PASS") == 0 && user->regi_status == 3)
    {
        if(user->regi_status == 0)
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
        }
        else
        {
            str = this->msg("irssi", "462", msg.params[0], "You may not reregister").c_str();
            len = str.length();
            send(user->client_fd,str.c_str(),len,0);
        }
            // throw "ERR_ALREADYREGISTERED";
    }
}
void Server::commandPath(ircMessage msg, Client * user)
{
	std::string		str;
	size_t			len;
    // std::cout << msg.command << std::endl;
    if(msg.params.size() > 0)
    {
        if(msg.command.compare("CAP") == 0)
        {

                if(msg.params[0].compare("LS") == 0)
                {
                    std::string ls = cap_ls();
                    str = this->msg("irssi", "CAP * LS", ls,"").c_str();
                    len = str.length();
                    send(user->client_fd,str.c_str(),len,0);
                }
                else if(msg.params[0].compare("REQ") == 0)
                {
                    // std::cout << msg.params[0] << std::endl;
                    str = this->msg("irssi", "CAP * ACK", cap_ack(msg),"").c_str();
                    len = str.length();
                    send(user->client_fd,str.c_str(),len,0);
                }
                else if(msg.params[0].compare("END") == 0)
                {
                    // if(user->regi_status == 1)
                    // {
                        str = this->msg("irssi", "001 user", "Welcome to the IRSSI.Chat Internet Relay Chat Network user","").c_str();
                        len = str.length();
                        send(user->client_fd,str.c_str(),len,0);
                    // }
                    // else
                    // {
                    //     str = this->msg("irssi", "464", msg.params[0], "Password incorrect").c_str();
                    //     len = str.length();
                    //     send(user->client_fd,str.c_str(),len,0);
                    //     close(user->client_fd);
                    //     user->client_fd = -1;
                    // }
                }
        }
            else if(msg.command.compare("PASS") == 0)
            {
                if(user->regi_status == 0)
                {
                    if(msg.params[0].compare(this->pass) == 0)
                    {
                        user->regi_status = 1;
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
                        // throw  "No re-registeration";
            }	
			else if(msg.command.compare("NICK") == 0)
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
            }
			else if(msg.command.compare("USER") == 0)
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
                }
                else
                {
                    str = this->msg("irssi", "462", msg.params[0], "You may not reregister").c_str();
                    len = str.length();
                    send(user->client_fd,str.c_str(),len,0);
                }
                    // throw "ERR_ALREADYREGISTERED";
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
            else if(msg.command.compare("JOIN") == 0)
            {
                if(user->regi_status == 0 && user->regi_status != 6)
                {
                    str = this->msg("irssi", "NOTOCE *", "***", "Checking Ident").c_str();
                    len = str.length();
                    send(user->client_fd,str.c_str(),len,0);
                    str = this->msg("irssi", "NOTOCE *", "***", "Looking up your hostname...").c_str();
                    len = str.length();
                    send(user->client_fd,str.c_str(),len,0);
                    str = this->msg("irssi", "NOTOCE *", "***", "No Ident response").c_str();
                    len = str.length();
                    send(user->client_fd,str.c_str(),len,0);
                    str = this->msg("irssi", "NOTOCE *", "***", "Couldn't look up your hostname").c_str();
                    len = str.length();
                    send(user->client_fd,str.c_str(),len,0);
                }
                //check if channel exists
                // else if()
                //check if channel doesnt exists and creats new one when it doesnt  
                // else if()
                
            }
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
	
int Server::Recv_end(int fd, std::string & line)
{
    char buffer[1024];
    memset(buffer, 0, 1024);
    int len = 0 ;
    int total = 0;
    while((len = recv(fd, buffer, 1, 0)) > 0 )
    {
        line = line + buffer;
        total += len; 
        if(line.find('\n') != std::string::npos || line.find("\r\n") != std::string::npos)
            break;
       memset(buffer, 0, 1024);
    }
    if(line.find("\r\n") != std::string::npos)
        line = line.substr(0,line.find("\r\n"));
    return total;
}
int Server::serverLoop()
{
    int j = 0;
	while(1)
    {
        j++;
        if (poll((&this->fd_poll[0]), this->number_of_clients, 1000) == -1)
        {
            std::cerr << "Error in poll" << std::endl;
            return 1;
        }
        connectionEvent();
        for(int i = 1; i < this->number_of_clients; i++)
        {
            if (this->fd_poll[i].revents & POLLIN )
            {
                std::string	line;
                int readed = this->Recv_end( this->client[i - 1]->client_fd,line);;
                if (readed > 0)
                {
                    std::cout << "Client " << i << " sent: " << line << std::endl;
					commandPath(parseMessage(line),this->client[i - 1]);
                    line.clear();
                }
                else if (readed <= 0 ){
                    std::cerr << "Client disconnected" << std::endl;
                    close(this->client[i - 1]->client_fd);
                    delete this->client[i - 1];
                    this->client.erase(this->client.begin() + i - 1);
                    this->fd_poll.erase(this->fd_poll.begin() + i);
                    this->number_of_clients--;
                }
                else
                    close(this->fd_poll[i].fd);
            }
        }
    }
    close(this->server);
    return 0;
}
/*extra*/
/*getter and setters*/
/*getter and setters*/

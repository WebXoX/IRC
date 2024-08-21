#include "../inc/Server.hpp"
#include "../inc/Channel.hpp"

/* orth Server / constructor*/
Server::Server ()
{
	std::cout << "Server default constructor" << std::endl;
}

Server::Server (std::string port , std::string pass)
{
    this->server_name = "ircserver";
    this->port = std::atoi(port.c_str());
    memset(&this->service, 0, sizeof(this->service));
    this->fd_poll.clear();
    this->client.clear();
    this->nicknames.clear();
    this->channels.clear();
    this->ports = port;
    this->pass = pass;
    this->creation_date.resize(80);
    this->creation_date = date_now((char *)this->creation_date.c_str());
    this->guestuser = 0;
    this->server = -1;
    this->number_of_clients = 0;

	std::cout << "Server default constructor" << std::endl;
}


Server::Server (const Server &a)
{
	std::cout << "Server copy constructor " << std::endl;
	*this = a;
}


Server::~Server ()
{
    free_delete();
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
    this->service.sin_family = AF_INET;
    this->service.sin_addr.s_addr = INADDR_ANY;
    service.sin_port = htons(this->port);
	pollfd poll;
    memset(&poll, 0, sizeof(poll));
	poll.fd = this->server;
	poll.events = POLLIN;
	this->fd_poll.push_back(poll);
    this->number_of_clients = 1;
	return 0;
}

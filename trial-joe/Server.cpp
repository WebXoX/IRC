#include "Server.hpp"

/* orth Server / constructor*/
Server::Server ()
{
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
int Server::serverInit()
{
    this->port = 27015;
    this->server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // fcntl(this->server, F_SETFL, O_NONBLOCK);
    this->client = -1;
	if(this->server == -1)
	{
		std::cerr << "Error creating socket" << std::endl;
		return 1;
	}
    this->service.sin_family = AF_INET;
    this->service.sin_addr.s_addr = INADDR_ANY;
    service.sin_port = htons(this->port);
	memset(&(this->fd_poll), 0, sizeof(this->fd_poll));
    this->fd_poll[0].fd = this->server;
    this->fd_poll[0].events = POLLIN;
    number_of_clients = 0;
	return 0;
}
int Server::runServer()
{
	if(bind(this->server, (sockaddr*)&this->service, sizeof(this->service)) == -1)
    {
        std::cerr << "Error binding socket" << std::endl;
        close(this->server);
        return 1;
    }
    if(listen(this->server, 1) == -1)
    {
        std::cerr << "Error listening on socket" << std::endl;
        close(this->server);
        return 1;
    }
    return 0;
}
int Server::serverLoop()
{
    int j = 0;
	while(1)
    {
        j++;
        poll((this->fd_poll), this->number_of_clients + 1, 500);
        for(int i = 0; i < this->number_of_clients + 1; i++)
        {
            if (this->fd_poll[0].revents & POLLIN && this->fd_poll[0].fd == this->server)
            {
                this->client = accept(this->server, nullptr, nullptr);
                // fcntl(this->client, F_SETFL, O_NONBLOCK);
                std::string message = "Hello from server";
                send(this->client, message.c_str(), message.size() + 1, 0);
                this->number_of_clients++;
            }
            if ( this->fd_poll[i].revents & POLLIN && i != 0 )
            {
                char buffer[1024];

                memset(buffer, 0, 1024);
                int bytes = recv(this->client, buffer, 1024, 0);
                if(bytes > 0)
                {
                    std::cout << "Received: " << std::string(buffer, 0, bytes) << std::endl;
                }
            }
            
        }
    }
    close(this->server);
    return 0;
}
/*extra*/
/*getter and setters*/
/*getter and setters*/

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
int Server::serverInit(int port, std::string pass)
{
    this->port = 27015;
    this->pass = pass;
    this->server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int flags = fcntl(this->server, F_GETFL, 0);
    if (fcntl(this->server, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        std::cerr << "Error setting socket to non-blocking" << std::endl;
        return 1;
    }
    // this->client = -1;
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
	memset(&(this->fd_poll), 0, sizeof(this->fd_poll));
    this->fd_poll[0].fd = this->server;
    this->fd_poll[0].events = POLLIN;
    number_of_clients = 1;
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
int Server::connectionEvent()
{
    if (this->fd_poll[0].revents & POLLIN)
    {
        this->client[this->number_of_clients - 1] = accept(this->server, nullptr, nullptr);
        if(this->client[this->number_of_clients - 1] == -1)
        {
            std::cerr << "Error accepting client" << std::endl;
            return 1;
        }
        int flags = fcntl(this->client[this->number_of_clients - 1], F_GETFL, 0);
        if (fcntl(this->client[this->number_of_clients - 1], F_SETFL, flags | O_NONBLOCK) == -1)
        {
            std::cerr << "Error setting client socket to non-blocking" << std::endl;
            return 1;
        }
        // fcntl(this->client, F_SETFL, O_NONBLOCK);
            std::cerr << " accepting client" << std::endl;

        std::string message = "Hello from server";
        send(this->client[this->number_of_clients - 1], message.c_str(), message.size() + 1, 0);
        this->fd_poll[number_of_clients].fd = this->client[this->number_of_clients - 1];
        this->fd_poll[number_of_clients].events = POLLIN | POLLOUT;
        this->number_of_clients++;
    }
    return 0;
}
int Server::serverLoop()
{
    int j = 0;
    char buffer[1024];

	while(1)
    {
        j++;
        if (poll((this->fd_poll), this->number_of_clients, 1000) == -1)
        {
            std::cerr << "Error in poll" << std::endl;
            return 1;
        }
        connectionEvent();
        for(int i = 1; i < this->number_of_clients; i++)
        {
            // std::cerr << "i " << i << std::endl;
            if (this->fd_poll[i].revents & POLLIN )
            {
                std::cout << "Received message from client" << std::endl;
                memset(buffer, 0, 1024);
                int bytes = recv(this->client[i - 1], buffer, 1024, 0);
                if(bytes > 0)
                {
                    std::cout << "Received: " << std::string(buffer, 0, bytes) << std::endl;
                }
            }
            if (this->fd_poll[i].revents & POLLHUP)
            {
                std::cerr << "Client disconnected" << std::endl;
                close(this->client[i - 1]);
                this->client[i - 1] = -1;
                for(int j = i; j < this->number_of_clients; j++)
                {
                    this->client[j - 1] = this->client[j];
                }
                this->number_of_clients--;
            }
        }
    }
    close(this->server);
    return 0;
}
/*extra*/
/*getter and setters*/
/*getter and setters*/

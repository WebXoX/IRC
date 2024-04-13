#include "../inc/Server.hpp"
#include "../inc/Channel.hpp"
#include <poll.h>
#include <stack>
#include <cmath>
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
		exit(0);
	}
	else if (sig == SIGTSTP)
	{
		exit(0);
	}
}


int Server::serverInit()
{
    // initization of server socket port poll 
    this->creation_date = date_now();
    this->server_name = "irssi";
    this->guestuser = 0;
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
    signal(SIGTSTP,  this->sighandle);
    signal(SIGINT, this->sighandle);
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
		poll.events = POLLIN | POLLOUT | POLLHUP;
		this->fd_poll.push_back(poll);
        this->number_of_clients++;
    }
    return 0;
}
std::string Server::msg(std::string source, std::string command, std::string param, std::string text)
{
    return (":"+source+ " " +command+" :"+param+" "+text + "\r\n");
}

int Server::definedmessage(int fd,std::string str)
{
    std::cout << "-------------------" << std::endl;
    std::cout << "Server send: " << str << std::endl;
    std::cout << "-------------------" << std::endl;

    if(send(fd, str.c_str(), str.length(), 0) == -1)
    {
        std::cerr << "Error sending message" << std::endl;
        return 1;
    }
    return 0;
}

std::string 	Server::date_now()
{
    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo = gmtime(&rawtime);
    
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%a %b %d %Y at %H:%M:%S UTC", timeinfo);
    return buffer;
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
	while(1)
    {
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
               
                int readed = this->Recv_end( this->client[i - 1]->client_fd,this->client[i - 1]->line);
                std::cout << "Client " << i << " sent l: :" << this->client[i - 1]->line[this->client[i - 1]->line.size()-1]<< ":" << std::endl;
                if (readed > 0)// && this->client[i - 1]->line[this->client[i - 1]->line.size() - 1] == '\n')
                {
                    std::cout << "Client " << i << " sent: " << this->client[i - 1]->line << std::endl;
					commandPath(parseMessage(this->client[i - 1]->line),this->client[i - 1]);
                    this->client[i - 1]->line.clear();
                }
                else if (readed <= 0 ){
                    std::cerr << "Client disconnected" << std::endl;
                    close(this->client[i - 1]->client_fd);
                    if(this->client[i - 1]->nickname.empty() == false)
                        this->nicknames.erase(std::find(this->nicknames.begin(), this->nicknames.end(), (this->client[i - 1]->nickname)));
                    delete this->client[i - 1];
                    this->client.erase(this->client.begin() + i - 1);
                    this->fd_poll.erase(this->fd_poll.begin() + i);
                    this->number_of_clients--;
                }
            }
            if (this->fd_poll[i].revents & POLLHUP)
            {
                std::cout << "Client " << i << " is ready to write" << std::endl;
            }
        }
    }
    close(this->server);
    return 0;
}
/*extra*/
/*getter and setters*/
/*getter and setters*/

bool Server::isUser(Client& user) {
    return std::find(this->client.begin(), this->client.end(), &user) != this->client.end();
}

bool Server::isUserNick(std::string nickname) {
    return std::find(this->nicknames.begin(), this->nicknames.end(), nickname) != this->nicknames.end();
}
int Server::getuser_fd(std::string name)
{
    for(int i = 0; i < this->number_of_clients; i++)
    {
        if(this->client[i]->nickname == name)
            return this->client[i]->client_fd;
    }
    return -1;
}
Client* Server::getClient(std::string nickname)
{
   for(int i = 0; i < this->number_of_clients; i++)
    {
        if(this->client[i]->nickname == nickname)
            return this->client[i];
    }
    Client *client =NULL;
    return client;
}
// ****** CHANNEL ****** //

int Server::addChannelInServer(Channel& channel) {
    if (hasChannelInServer(channel.getName())) 
        return 1;
    this->channels[channel.getName()] = channel;
    return 0;
}

bool Server::hasChannelInServer(std::string channelName) {
    return this->channels.find(channelName) != this->channels.end();
}






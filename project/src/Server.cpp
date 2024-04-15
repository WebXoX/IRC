#include "../inc/Server.hpp"
#include "../inc/Channel.hpp"

bool server_status = true;

void	sighandle(int sig)
{
	if (sig == SIGINT)
	{
		server_status = false;
	}
	else if (sig == SIGTSTP)
	{
        server_status = false;
	}
}
/* orth Server / constructor*/
Server::Server ()
{
	std::cout << "Server default constructor" << std::endl;
}


Server::Server (std::string port , std::string pass)
{
    this->server_name = "irssi";
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
std::string Server::msg(std::string source, std::string command, std::string param, std::string text)
{
    return (":"+source+ " " +command+" :"+param+" "+text + "\r\n");
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
    signal(SIGTSTP, sighandle);
    signal(SIGINT, sighandle);
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
        new_client->port = this->port;
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

int Server::serverLoop()
{
	while(server_status == true)
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
                if (readed > 0 && this->client[i - 1]->line[this->client[i - 1]->line.size() - 1] == '\n')
                {
					commandPath(parseMessage(this->client[i - 1]->line),this->client[i - 1]);
                    this->client[i - 1]->line.clear();
                }
                else if (readed <= 0 ){
                    std::cerr << "Client:- " << this->client[i - 1]->nickname << " disconnected" << std::endl;
                    close(this->client[i - 1]->client_fd);
                    quitCmd("",this->client[i - 1]);
                    if(this->client[i - 1]->nickname.empty() == false)
                        this->nicknames.erase(std::find(this->nicknames.begin(), this->nicknames.end(), (this->client[i - 1]->nickname)));
                    delete this->client[i - 1];
                    this->client.erase(this->client.begin() + i - 1);
                    this->fd_poll.erase(this->fd_poll.begin() + i);
                    this->number_of_clients--;
                }
            }
            else if (this->fd_poll[i].revents & POLLOUT )
            {
                this->definedmessage(this->client[i - 1]);
            }
        }
    }
    close(this->server);
    return 0;
}

int Server::definedmessage(Client *user)
{
    for (size_t i = 0; i < user->toSend.size(); i++)
    {
        std::cout << "-------------------" << std::endl;
        std::cout << "Server "<< user->nickname << " send: " << user->toSend[i] ;
        std::cout << "-------------------" << std::endl;
        if (send(user->client_fd, user->toSend[i].c_str(), user->toSend[i].length(), 0) == -1)
        {
            std::cerr << "Error sending message" << std::endl;
            return 1;
        }
    }
    return 0;
}

std::string 	Server::date_now(char * buffer)
{
    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo = gmtime(&rawtime);
    
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
    return total;
}
void Server::free_delete()
{
    for(size_t i = 0; i < this->client.size(); i++)
    {
        close(this->client[i]->client_fd);
        delete this->client[i];
    }
}
/*extra*/

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

std::map<std::string, Channel>::iterator	Server::getChannelIt(std::string chanName) {
    return this->channels.find(chanName);
}

Client* Server::getUser(std::string name) {
    for (size_t i = 0; i != this->client.size(); i++) {
        if (this->client[i]->nickname == name )
            return this->client[i];
    }
    return NULL;
}
/*getter and setters*/


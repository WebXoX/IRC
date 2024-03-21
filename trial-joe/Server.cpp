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
    // initization of server socket port poll 
    this->port = 27015;
    this->pass = pass;
    this->server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // int flags = fcntl(this->server, F_GETFL, 0);
    // if (fcntl(this->server, F_SETFL, flags | O_NONBLOCK) == -1)
    // {
    //     std::cerr << "Error setting socket to non-blocking" << std::endl;
    //     return 1;
    // }
    // this->client = -1;
	if(this->server == -1)
	{
		std::cerr << "Error creating socket" << std::endl;
		return 1;
	}
    //socket config
	// memset(&(this->service), 0, sizeof(this->service));
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
// int Server::connectionEvent()
// {
//     // checking for new connections events
//     if (this->fd_poll[0].revents & POLLIN)
//     {
//         this->client[this->number_of_clients - 1] = accept(this->server, nullptr, nullptr);
//         if(this->client[this->number_of_clients - 1] == -1)
//         {
//             std::cerr << "Error accepting client" << std::endl;
//             return 1;
//         }
//         int flags = fcntl(this->client[this->number_of_clients - 1], F_GETFL, 0);
//         if (fcntl(this->client[this->number_of_clients - 1], F_SETFL, flags | O_NONBLOCK) == -1)
//         {
//             std::cerr << "Error setting client socket to non-blocking" << std::endl;
//             return 1;
//         }
//         std::cerr << " accepting client" << std::endl;
//         // client registration 

//         std::string message = "Hello from server";

//         send(this->client[this->number_of_clients - 1], message.c_str(), message.size() + 1, 0);
//         // setting poll for tracking events of the new client
//         this->fd_poll[number_of_clients].fd = this->client[this->number_of_clients - 1];
//         this->fd_poll[number_of_clients].events = POLLIN | POLLOUT;
//         this->number_of_clients++;
//     }
//     return 0;
// }
//  int           id_check(Client *user)
//  {
//     user->addrlen = sizeof(user->my_addr);
//     if (getsockname(user->client_fd, (struct sockaddr *) & user->my_addr, &user->addrlen ) == -1) {
//         perror("getsockname");
//         return 1;
//     }
//     user->ip =  std::string(inet_ntoa(user->my_addr.sin_addr));
//     user->port = ntohs(user->my_addr.sin_port);
//     return 1;
//  }
// int           cap_ls(Client *user)
//  {

//  }
//  int           id_check(Client *user)
//  {

//  }

int             Server::id_check(Client *user)
{
    if(send(user->client_fd,msg("irssi","NOTICE *","*** ","Checking Ident").c_str(), msg("irssi","NOTICE *","***","Checking Ident").length(), 0) <= 0)
    {
        std::cerr << "Error sending message" << std::endl;
        return -1;
    }
    if(send(user->client_fd,msg("irssi","NOTICE *","*** ","Looking up your hostname...").c_str(), msg("irssi","NOTICE *","***","Looking up your hostname...").length(), 0) <= 0)
    {
        std::cerr << "Error sending message" << std::endl;
        return -1;
    }
    if(send(user->client_fd,msg("irssi","NOTICE *","*** ","No Ident response").c_str(), msg("irssi","NOTICE *","***","No Ident response").length(), 0) <= 0)
    {
        std::cerr << "Error sending message" << std::endl;
        return -1;
    }
    if(send(user->client_fd,msg("irssi","NOTICE *","*** ","Couldn't look up your hostname").c_str(), msg("irssi","NOTICE *","***","Couldn't look up your hostname").length(), 0) <= 0)
    {
        std::cerr << "Error sending message" << std::endl;
        return -1;
    }
    return 1;
}

std::string Server::cap_ls()
{
    
    return ("account-notify away-notify chghost extended-join multi-prefix sasl=PLAIN server-time");
}
// std::string Server::welcome_msg()
// {

//     return ();
// }
int Server::register_user(Client *user /*, Parsing msg*/)
{
    std::string line;
    if(line.compare("CAP LS 302") == 0)
    {
        id_check(user);
        send (user->client_fd, cap_ls().c_str(), cap_ls().length(), 0);
    }
    if(line.find("CAP REQ ") != std::string::npos)
    {
    }

}


// int Server::register_user(Client *user /*, Parsing msg*/)
// {
//     std::string line;
//     char host[1000];
//     char service[1000];
//     int bytes = this->Recv_end(user->client_fd, line); 
//     if(bytes > 0)
//     {
//         // if(line.compare("CAP LS 302") == 0)
//         // {
//             // id_check(user);
//             // cap_ls(user);
//             line.clear();
//             int bytes = this->Recv_end(user->client_fd,line);
//             struct sockaddr_in my_addr;
//             socklen_t addrlen = sizeof(my_addr);
//             if (getsockname(user->client_fd, (struct sockaddr *) &my_addr, &addrlen) == -1) {
//                 perror("getsockname");
//                 return 1;
//             }
//             // if(gethostname())
//             socklen_t  len = sizeof my_addr;
//             getnameinfo((sockaddr *)&my_addr, sizeof my_addr, host, sizeof host, service, sizeof service, 0);
//             std::cout << "sin_addr: " <<   inet_ntoa(my_addr.sin_addr) << std::endl;
//             std::cout << "sin_addr: " << my_addr.sin_family << std::endl;
//             std::cout << "sin_addr: " << ntohs(my_addr.sin_port)<< std::endl;
//             std::cout << "sin_addr: " << my_addr.sin_zero << std::endl;

//             struct hostent *he;
//         struct in_addr ipv4addr;
//         struct in6_addr ipv6addr;

//         inet_pton(AF_INET, user->ip.c_str(), &ipv4addr);
//         he = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);
//         printf("Host name: %s\n", he->h_name);
//     // Extract the hostname from the result
//         std::string message = "Checking Ident";
//             if(send(user->client_fd,msg("irssi","NOTICE *","*** ",message).c_str(), msg("irssi","NOTICE *","***",message).length(), 0) <= 0)
//             {
//                 std::cerr << "Error sending message" << std::endl;
//                 return -1;
//             }
//         message = "Looking up your hostname...";
//             if(send(user->client_fd,msg("irssi","NOTICE *","*** ",message).c_str(), msg("irssi","NOTICE *","***",message).length(), 0) <= 0)
//             {
//                 std::cerr << "Error sending message" << std::endl;
//                 return -1;
//             }
//         message = "No Ident response"; 
//             if(send(user->client_fd,msg("irssi","NOTICE *","*** ",message).c_str(), msg("irssi","NOTICE *","***",message).length(), 0) <= 0)
//             {
//                 std::cerr << "Error sending message" << std::endl;
//                 return -1;
//             }
//          message = "Couldn't look up your hostname";
//             if(send(user->client_fd,msg("irssi","NOTICE *","*** ",message).c_str(), msg("irssi","NOTICE *","***",message).length(), 0) <= 0)
//             {
//                 std::cerr << "Error sending message" << std::endl;
//                 return -1;
//             }
//         // }
//     }  
//     return 1; 
// }
int Server::connectionEvent()
{
    // checking for new connections events
    if (this->fd_poll[0].revents & POLLIN)
    {
        Client *new_client = new Client();
        new_client->client_fd = accept(this->server, nullptr, nullptr);
        if(new_client->client_fd == -1)
        {
            std::cerr << "Error accepting client" << std::endl;
            return 1;
        }
        int flags = fcntl(new_client->client_fd, F_GETFL, 0);
        if (fcntl(new_client->client_fd, F_SETFL, flags | O_NONBLOCK) == -1)
        {
            std::cerr << "Error setting client socket to non-blocking" << std::endl;
            return 1;
        }
        this->client.push_back(new_client);
        std::cerr << " accepting client" << std::endl;
        // client registration 
        this->register_user(new_client);
        this->fd_poll[number_of_clients].fd = new_client->client_fd;
        this->fd_poll[number_of_clients].events = POLLIN | POLLOUT | POLLERR | POLLHUP;;
        this->number_of_clients++;
    }
    return 0;
}
// int Server::serverLoop()
// {
//     int j = 0;
//     char buffer[1024];

// 	while(1)
//     {
//         j++;
//         if (poll((this->fd_poll), this->number_of_clients, 1000) == -1)
//         {
//             std::cerr << "Error in poll" << std::endl;
//             return 1;
//         }
//         connectionEvent();
//         for(int i = 1; i < this->number_of_clients; i++)
//         {
//             if (this->fd_poll[i].revents & POLLIN )
//             {
//                 std::cout << "Received message from client" << std::endl;
//                 memset(buffer, 0, 1024);
//                 int bytes = recv(this->client[i - 1], buffer, 1024, 0);
//                 if(bytes > 0)
//                 {
//                     std::cout << "Received: " << std::string(buffer, 0, bytes) << std::endl;
//                 }
//             }
//             if (this->fd_poll[i].revents & POLLHUP)
//             {
//                 std::cerr << "Client disconnected" << std::endl;
//                 close(this->client[i - 1]);
//                 this->client[i - 1] = -1;
//                 for(int j = i; j < this->number_of_clients; j++)
//                 {
//                     this->client[j - 1] = this->client[j];
//                 }
//                 this->number_of_clients--;
//             }
//         }
//     }
//     close(this->server);
//     return 0;
// }
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
    std::string str;
	while(1)
    {
        j++;
        if (poll((this->fd_poll), this->number_of_clients, 100) == -1)
        {
            std::cerr << "Error in poll" << std::endl;
            return 1;
        }
        connectionEvent();
        for(int i = 1; i < this->number_of_clients; i++)
        {
            // if (this->fd_poll[i].revents &  (POLLERR | POLLRDHUP))
            // {
            //     std::cerr << "Client disconnected" << std::endl;
            //     close(this->client[i - 1]->client_fd);
            //     delete this->client[i - 1];
            //     this->client.erase(this->client.begin() + i - 1);
            //     for (int j = i + 1; j < this->number_of_clients; j++)
            //     {
            //         this->fd_poll[i].fd = this->fd_poll[j].fd;
            //         this->fd_poll[i].events = this->fd_poll[j].events;
            //         i++; // Increment i to overwrite the next element
            //     }
            //     this->number_of_clients--;
            // }
            
            if (this->fd_poll[i].revents & POLLIN )
            {
                std::string	line;
                int readed = this->Recv_end( this->client[i - 1]->client_fd,line);;
                if (readed > 0){
                    std::cout << "Received: " << line;
                    str.clear();
                }
                else if (readed <= 0 ){
                    std::cerr << "Client disconnected" << std::endl;
                    close(this->client[i - 1]->client_fd);
                    delete this->client[i - 1];
                    this->client.erase(this->client.begin() + i - 1);
                    for (int j = i + 1; j < this->number_of_clients; j++)
                    {
                        this->fd_poll[i].fd = this->fd_poll[j].fd;
                        this->fd_poll[i].events = this->fd_poll[j].events;
                        i++; // Increment i to overwrite the next element
                    }
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

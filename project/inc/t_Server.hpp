#ifndef T_SERVER_HPP
#define T_SERVER_HPP


#include "common.hpp"
#include "Channel.hpp"
#include "Parser.hpp"
#include "Request.hpp"


class Channel;
class User;
class Parser;

class Server
{
private:
    SOCKET serverSocket;
    sockaddr_in servAddr;
    std::vector<pollfd> pollFds;
    pollfd serverPollFd;
    std::map<int, User> usersList;
    std::map<std::string, Channel> channels;
public:
    Parser parser;
    Request request;
    Server();
    ~Server();
    SOCKET setupSocket();
    SOCKET runServer(); 
    int acceptNewConnection();
    int processRequest(int pollIndex);
    int addUser(int client_fd);
    int removeUser(int client_fd);
    int addPollFd(int client_fd); 

    //commands
    std::string processCommand(int client_fd);
    std::string joinChannel(int client_fd);
};

std::string Server::joinChannel(int client_fd) {
    std::map<int, User>::iterator it = this->usersList.find(client_fd);
    if (it != this->usersList.end()) {
        send(client_fd, "hello from join", 50, 0);
    }
    return "";
}

std::string Server::processCommand(int client_fd) {
    if (!request.isValidInput)
        return "Invalid Message";
    else if (request.command == "JOIN")
        return (joinChannel(client_fd));
    return "";
}


Server::Server()
{
    int reSock = this->setupSocket();
    if (reSock == INVALID_SOCKET)
        perror("Socket Error: ");
    reSock = runServer();
    if (reSock == INVALID_SOCKET)
        perror("Server Error: ");
}

Server::~Server()
{
}


SOCKET Server::setupSocket()
{
    // create a socket
    this->serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (this->serverSocket == INVALID_SOCKET)
        return INVALID_SOCKET;
    std::cout << "Socket created successfully" << std::endl;

    // set it to non-blocking mode
    if ((fcntl(this->serverSocket, F_SETFL, O_NONBLOCK) == INVALID_SOCKET))
        return INVALID_SOCKET;
    std::cout << "Socket set to non-blocking" << std::endl;

    // create a socket address structure
    memset(this->servAddr.sin_zero, 0, sizeof(this->servAddr.sin_zero));
    this->servAddr.sin_family = AF_INET;
    this->servAddr.sin_port = htons(PORT);
    this->servAddr.sin_addr.s_addr = INADDR_ANY;

    // set the socket options
    int optval = 1;
    if (setsockopt(this->serverSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
        return INVALID_SOCKET;
    std::cout << "Socket options set successfully" << std::endl;

    // bind the socket to the address and port
    if ((bind(this->serverSocket, (sockaddr*)&this->servAddr, sizeof(this->servAddr)) == INVALID_SOCKET))
        return INVALID_SOCKET;
    std::cout << "Socket binded successfully" << std::endl;

    // set the socket to listen for incoming connections
    if ((listen(this->serverSocket, 1000) == INVALID_SOCKET))
        return INVALID_SOCKET;
    std::cout << "Socket is listening for incoming connections" << std::endl;

    return this->serverSocket;
}

int Server::addUser(int client_fd) {
    User newUser(client_fd, "user" + std::to_string(client_fd));
    usersList.insert(std::make_pair(client_fd, newUser));
    std::cout << newUser.getname() << " added succesfully" << std::endl;
    return 0;
}

int Server::addPollFd(int client_fd) {
    std::cout << "Client: " + std::to_string(client_fd) << " connected" << std::endl;
    send(client_fd, "Welcome to the server\n", 22, 0);
    pollfd clientPollFd;
    clientPollFd.fd = client_fd;
    clientPollFd.events = POLLIN;
    this->pollFds.push_back(clientPollFd);   

    // create a new user and add it to the users list
    this->addUser(client_fd);
    return 0;
}

int Server::acceptNewConnection()
{
        SOCKET clientSocket = accept(this->serverSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) {
            perror("Accept Error: ");
            return 1;
        }
        this->addPollFd(clientSocket);
        
        return 0;
}

int Server::removeUser(int client_fd) {
    if (this->usersList.find(client_fd) != this->usersList.end()) {
        this->usersList.erase(this->usersList.find(client_fd));
        std::cout << "User " << client_fd << "removed succesfully" << std::endl;
        return 0;
    }
    return 1;
}

int Server::processRequest(int pollIndex)
{
    char buffer[BUFFER_SIZE];
    int client_fd = this->pollFds[pollIndex].fd;
    int nbyte = recv(client_fd, buffer, sizeof(buffer), 0);
    if (nbyte <= 0) {
		close(client_fd);
		this->pollFds.erase(this->pollFds.begin() + pollIndex);
        if (this->removeUser(client_fd))
            perror("Could not remove user");
    }

    this->parser.parsing(buffer);
    this->request = parser.getRequest();

    std::string ret = this->processCommand(client_fd);
    send()

    std::cout << "command: " << request.command << std::endl;
    std::cout << "Arg: " << request.args[0] << std::endl;
    memset(&buffer, 0, sizeof(buffer));
    return 0;
}

SOCKET Server::runServer() {

    this->serverPollFd.fd = this->serverSocket;
    this->serverPollFd.events = POLLIN;
    this->pollFds.push_back(this->serverPollFd);
     while (true)
    {
        int pollReady = poll(&this->pollFds[0], this->pollFds.size(), -1);

        if (pollReady == -1) {
            perror("Poll Error: ");
            return 1;
        }

        for (size_t i = 0; i < this->pollFds.size(); i++) {
            if (this->pollFds[i].revents & POLLIN) {
                if (this->pollFds[i].fd == this->serverSocket)
                    this->acceptNewConnection();
                else
                    this->processRequest(i);
            }
        }
    }
 }


#endif
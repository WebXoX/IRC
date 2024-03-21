

#include "../inc/parsing.hpp"
#include "Channel.hpp"

// class Channel;
// class User;

#define PORT 4242
#define SOCKET int
#define INVALID_SOCKET -1
#define BUFFER_SIZE 1024


class Server_t
{
private:
    SOCKET serverSocket;
    sockaddr_in servAddr;
    std::vector<pollfd> pollFds;
    std::map<int, User> usersList;
    std::map<std::string, Channel> channelsList;
    pollfd serverPollFd;
public:
    Server_t();
    ~Server_t();
    SOCKET setupSocket();
    SOCKET runServer();
};

Server_t::Server_t()
{
    int reSock = this->setupSocket();
    if (reSock == INVALID_SOCKET)
        perror("Socket Error: ");
    reSock = runServer();
    if (reSock == INVALID_SOCKET)
        perror("Server Error: ");
}

Server_t::~Server_t()
{
}


SOCKET Server_t::setupSocket()
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

SOCKET Server_t::runServer() {

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
                if (this->pollFds[i].fd == this->serverSocket) {
                    SOCKET clientSocket = accept(this->serverSocket, NULL, NULL);
                    if (clientSocket == INVALID_SOCKET) {
                        perror("Accept Error: ");
                        return 1;
                    }
                    std::cout << "Client: " + std::to_string(clientSocket) << " connected" << std::endl;
                    send(clientSocket, "Welcome to the server", 21, 0);
                    pollfd clientPollFd;
                    clientPollFd.fd = clientSocket;
                    clientPollFd.events = POLLIN;
                    this->pollFds.push_back(clientPollFd);

                    // create a new user and add it to the users list
                    User newUser(clientSocket, "User", "User" + std::to_string(clientSocket), "IRSSI");
                    this->usersList.insert(std::make_pair(clientSocket, newUser));

                } else {
                    char buffer[BUFFER_SIZE];
                    int bytesRead = recv(this->pollFds[i].fd, buffer, BUFFER_SIZE, 0);
                    if (bytesRead <= 0) {
                        if (bytesRead == 0) {
                            std::cout << "Client disconnected" << std::endl;
                        } else {
                            perror("Recv Error: ");
                        }
                        close(this->pollFds[i].fd);
                        this->pollFds.erase(this->pollFds.begin() + i);
                    } else {
                        ircMessage message = parseMessage(buffer);
                        if (message.command == "JOIN") {
                            // check if the channel exists in the channel map at the server
                            // param 0 is the channel name is the placeholder for the channel object
                            std::map<std::string, Channel>::iterator itChan = this->channelsList.find(message.params[0]);
                            std::map<int, User>::iterator itUser = this->usersList.find(this->pollFds[i].fd);
                            if (itChan != this->channelsList.end()){
                                itChan->second.addUser(itUser->second);
                            } else {
                                Channel newChannel(message.params[0], itUser->second);
                                this->channelsList.insert(std::pair<std::string, Channel>(message.params[0], newChannel));
                            }
                        }
                        memset(buffer, 0, BUFFER_SIZE);
                    }
                }
            }
    
        }
    }
}

int main()
{
    Server_t server;


    return 0;
}
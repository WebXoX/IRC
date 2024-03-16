

#include "../inc/parsing.hpp"

SOCKET setupSocket()
{
    // create a socket
    SOCKET serverSocket;
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        perror("Socket Error: ");
        return INVALID_SOCKET;
    }
    std::cout << "Socket created successfully" << std::endl;

    // set it to non-blocking mode
    if ((fcntl(serverSocket, F_SETFL, O_NONBLOCK) == INVALID_SOCKET)) {
        perror("Fcntl Error: ");
        return INVALID_SOCKET;
    }
    std::cout << "Socket set to non-blocking" << std::endl;

    // create a socket address structure
    sockaddr_in servAddr;
    memset(servAddr.sin_zero, 0, sizeof(servAddr.sin_zero));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(PORT);
    servAddr.sin_addr.s_addr = INADDR_ANY;

    int optval = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
        perror("Setsockopt Error: ");
        close(serverSocket);
        return 1;
    }
    std::cout << "Socket options set successfully" << std::endl;

    // bind the socket to the address and port
    if ((bind(serverSocket, (sockaddr*)&servAddr, sizeof(servAddr)) == INVALID_SOCKET)) {
        perror("Bind Error: ");
        return INVALID_SOCKET;
    }
    std::cout << "Socket binded successfully" << std::endl;

    // set the socket to listen for incoming connections
    if ((listen(serverSocket, 1000) == INVALID_SOCKET)) {
        perror("Listen Error: ");
        return INVALID_SOCKET;
    }
    std::cout << "Socket is listening for incoming connections" << std::endl;
    return serverSocket;
}

int main()
{

    // setup the server socket
    SOCKET serverSocket = setupSocket();
    if (serverSocket == INVALID_SOCKET) {
        perror("Server setup failed: ");
        return 1;
    }


    std::vector<pollfd> pollFds;
    pollfd serverPollFd;
    serverPollFd.fd = serverSocket;
    serverPollFd.events = POLLIN;
    pollFds.push_back(serverPollFd);

    while (true)
    {
        int pollReady = poll(&pollFds[0], pollFds.size(), -1);

        if (pollReady == -1) {
            perror("Poll Error: ");
            return 1;
        }

        for (size_t i = 0; i < pollFds.size(); i++) {
            if (pollFds[i].revents & POLLIN) {
                if (pollFds[i].fd == serverSocket) {
                    SOCKET clientSocket = accept(serverSocket, NULL, NULL);
                    if (clientSocket == INVALID_SOCKET) {
                        perror("Accept Error: ");
                        return 1;
                    }
                    std::cout << "Client connected" << std::endl;
                    pollfd clientPollFd;
                    clientPollFd.fd = clientSocket;
                    clientPollFd.events = POLLIN;
                    pollFds.push_back(clientPollFd);
                } else {
                    char buffer[BUFFER_SIZE];
                    int bytesRead = recv(pollFds[i].fd, buffer, BUFFER_SIZE, 0);
                    if (bytesRead <= 0) {
                        if (bytesRead == 0) {
                            std::cout << "Client disconnected" << std::endl;
                        } else {
                            perror("Recv Error: ");
                        }
                        close(pollFds[i].fd);
                        pollFds.erase(pollFds.begin() + i);
                    } else {
                        printCommand(parseMessage(buffer));
                        memset(buffer, 0, BUFFER_SIZE);
                    }
                }
            }
        }
    
    }
    return 0;
}
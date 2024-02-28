#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <poll.h>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>

int main()
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct pollfd mypoll;

    memset(&mypoll, 0, sizeof(mypoll));
    mypoll.fd = socket_fd;
    mypoll.events = POLLIN;

    if (socket_fd == -1)
    {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }
    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = INADDR_ANY;
    int client = -1;
    // InetPton(AF_INET,_T("127.0.0.1"),&service.sin_addr);
    // inet_pton(AF_INET, "127.0.0.1", &service.sin_addr);
    service.sin_port = htons(27015);
    if(bind(socket_fd, (sockaddr*)&service, sizeof(service)) == -1)
    {
        std::cerr << "Error binding socket" << std::endl;
        close(socket_fd);
        return 0;
    }
    if(listen(socket_fd, 1) == -1)
    {
        std::cerr << "Error listening on socket" << std::endl;
        close(socket_fd);
        return 0;
    }
    while(1)
    {
         poll(&mypoll, 1, 100);
                    std::cout << "client " <<client  << std::endl;
            
                if (mypoll.revents & POLLIN && client == -1)
                {
                    std::cout << "connection"  << std::endl;
                    client = accept(socket_fd, nullptr, nullptr);
                    std::string message = "Hello from server";
                    send(client, message.c_str(), message.size() + 1, 0);
                }
                // if (client != -1 && mypoll.revents & POLLIN)
                // {
                //     std::cout << "connection2"  << std::endl;

                    char buffer[1024];
                    memset(buffer, 0, 1024);
                    int bytes = recv(client, buffer, 1024, 0);
                    if(bytes <= 0)
                    {
                    std::cout << "connection3"  << std::endl;
                        close(client);
                        client = -1;
                    }
                    else
                    {
                        std::cout << "Received: " << std::string(buffer, 0, bytes) << std::endl;
                    }
        //  }
        
    }
    close(socket_fd);
    return 0;
}
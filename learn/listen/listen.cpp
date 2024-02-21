#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_fd == -1)
    {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }
    sockaddr_in service;
    service.sin_family = AF_INET;
    // InetPton(AF_INET,_T("127.0.0.1"),&service.sin_addr);
    inet_pton(AF_INET, "127.0.0.1", &service.sin_addr);

    service.sin_port = htons(27015);
    if(bind(socket_fd, (sockaddr*)&service, sizeof(service)) == -1)
    {
        std::cerr << "Error binding socket" << std::endl;
        close(socket_fd);
        return 0;
    }
    else
    {
        std::cout << "Socket bound" << std::endl;
    }
    if(listen(socket_fd, 1) == -1)
    {
        std::cerr << "Error listening on socket" << std::endl;
        close(socket_fd);
        return 0;
    }
    else
    {
        std::cout << "Listening on socket" << std::endl;        
    }
    close(socket_fd);
    return 0;
}
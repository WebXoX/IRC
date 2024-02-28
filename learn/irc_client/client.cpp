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

    // service.sin_port = htons(3490);
    service.sin_port = htons(27015);
    if(connect(socket_fd, (sockaddr*)&service, sizeof(service)) == -1)
    {
        std::cerr << "Error connecting socket" << std::endl;
        close(socket_fd);
        return 0;
    }
            std::cout << "Message sent" << std::endl;
            char buffer[200] = "";
            if(recv(socket_fd, buffer, 1000, 0) <= 0)
            {
                std::cerr << "Error receiving message" << std::endl;
                close(socket_fd);
                return -1;
            }
            else
            {
                std::cout << buffer << std::endl;
            }
    while (1)
    {
        std::string message;
        std::cout << "Enter message: ";
        std::getline(std::cin, message);
        if(message == "exit" || message.empty())
        {
            break;
        }
        if(send(socket_fd, message.c_str(), message.size() + 1, 0) <= 0)
        {
            std::cerr << "Error sending message" << std::endl;
            close(socket_fd);
            return -1;
        }
    }
        
    close(socket_fd);
    return 0;
}
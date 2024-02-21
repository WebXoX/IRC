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
    if(connect(socket_fd, (sockaddr*)&service, sizeof(service)) == -1)
    {
        std::cerr << "Error connecting socket" << std::endl;
        close(socket_fd);
        return 0;
    }
    else
    {
        std::cout << "connected to server" << std::endl;
        char buffer[200] = "";
        if(recv(socket_fd, buffer, 13, 0) <= 0)
        {
            std::cerr << "Error receiving message" << std::endl;
            close(socket_fd);
            return -1;
        }
        else
        {
            std::cout << buffer << std::endl;
            if(send(socket_fd, "received", 13, 0) <= 0)
            {
                std::cerr << "Error sending message" << std::endl;
                close(socket_fd);
                return -1;
            }
            else
            {
                std::cout << "Message sent" << std::endl;
                char  buffer[200] = "";
                std::string buf;

                if(recv(socket_fd, (char *)buf.c_str(), 1000, 0) <= 0)
                {
                    std::cerr << "Error receiving message" << std::endl;
                    close(socket_fd);
                    return -1;
                }
                else
                {
                    std::cout << buf << std::endl;
                }
            }
        }
    }
    close(socket_fd);
    return 0;
}
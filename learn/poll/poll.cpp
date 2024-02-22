#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>

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
        if(listen(socket_fd, 1) == -1)
        {
            std::cerr << "Error listening on socket" << std::endl;
            close(socket_fd);
            return 0;
        }
        
    }
    fd_set current, ready_sockets;
    int clients;

    FD_ZERO(&current);
    FD_SET(socket_fd, &current);
    while(1)
    {
        ready_sockets = current;
        if(select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0)
        {
            std::cerr << "Error selecting socket" << std::endl;
            close(socket_fd);
            // return 0;
        }
        for(int i = 0; i < FD_SETSIZE; i++)
        {
            // std::cout << "Checking socket" << std::endl;
            if(FD_ISSET(i,&ready_sockets))
            {
                if(i == socket_fd)
                {
                    std::cout << "Listening on socket" << std::endl;
                    clients = accept(socket_fd, NULL, NULL);
                    if (clients == -1)
                    {
                        std::cerr << "Error accepting connection" << std::endl;
                        close(socket_fd);
                        // return -1;
                        FD_SET(clients, &current);
                    }
                    else
                    {
                        if(send(clients, "Hello, world!", 13, 0) <= 0)
                        {
                            std::cerr << "Error sending message" << std::endl;
                            close(socket_fd);
                            // return -1;
                        }
                        else
                        {
                            std::cout << "Message sent" << std::endl;
                            char  buffer[200] = "";
                            if(recv(clients, buffer, 13, 0) <= 0)
                            {
                                std::cerr << "Error receiving message" << std::endl;
                                close(socket_fd);
                                return -1;
                            }
                            else
                            {
                                std::cout << buffer << std::endl;
                                std::cout << "hi" << std::endl;
                                if(send(clients, "sent ", 13, 0) <= 0)
                                {
                                    std::cerr << "Error sending message" << std::endl;
                                    close(socket_fd);
                                    return -1;
                                }
                                else
                                {
                                    std::cout << "Message sent" << std::endl;
                                }
                                std::cout << "bye" << std::endl;

                            }
                        }
                        FD_CLR(i, &current);

                    }
                        std::cout << "bye" << std::endl;
                }
                        std::cout << "bye" << std::endl;

            }
            
        }
    }
    close(socket_fd);
    return 0;
}
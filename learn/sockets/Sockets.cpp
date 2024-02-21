#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <string>
#include <netinet/in.h>

//  socket(int socket_family, int socket_type, int protocol);
//  int socket(int domain, int type, int protocol);
//  for more info :-> https://manual.cs50.io/2/socket
//  for some more :-> https://manual.cs50.io/7/socket

/*
protocol modules are grouped into protocol families such as:
socket_family: AF_INET, AF_INET6, AF_UNIX
AF_INET: IPv4
AF_INET6: IPv6
AF_UNIX: Local communication
*/
/*
socket_type: SOCK_STREAM, SOCK_DGRAM, SOCK_SEQPACKET
SOCK_STREAM: TCP
SOCK_DGRAM: UDP
SOCK_SEQPACKET: TCP
*/
/*
for more info :-> https://manual.cs50.io/5/protocols
use this command in terminal to see the numbers for protocols
```
cat /etc/protocols
```
protocol: IPPROTO_TCP, IPPROTO_UDP
IPPROTO_TCP: TCP
IPPROTO_UDP: UDP
*/
int main()
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_fd == -1)
    {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }
    close(socket_fd);
    return 0;
}
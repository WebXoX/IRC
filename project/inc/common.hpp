#ifndef __COMMON_HPP__
#define __COMMON_HPP__

#include <iostream>
#include <cerrno>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <poll.h>
#include <cstdio>
#include <sstream>

#define PORT 4242
#define SOCKET int
#define INVALID_SOCKET -1
#define BUFFER_SIZE 1024

#endif
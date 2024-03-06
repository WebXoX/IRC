#pragma once
#ifndef __SERVER_HPP__
#define __SERVER_HPP__
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <poll.h>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <map>
#include <fcntl.h>

/*
*/
class Server
{
	/* VARIABLES*/
	private:
		int server;
		// std::map<std::string, int> client;
		int client[1024];
		int number_of_clients;
		int port;
		std::string pass;
		sockaddr_in service;
		struct pollfd fd_poll[10241];
    /* VARIABLES*/
	public:
	/*orth form*/
    	Server ();
    	Server (const Server &a);
    	~Server ();
		Server& operator=(const Server &a);
	/*orth Server*/
	/*exception*/
		int serverInit(int port , std::string pass);
		int runServer();
		int	serverLoop();
		int	connectionEvent();



	
	/*exception*/
	/*getters and setters*/
	/*getters and setters*/
	/*extra*/
	/*extra*/
};
#endif

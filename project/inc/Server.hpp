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
#include <cstring>
#include "../inc/Client.hpp"
#include "../inc/parsing.hpp"


#include <netdb.h>
/*
*/
class Server
{
	/* VARIABLES*/
	private:
		int server;
		std::vector <Client*> client;
		// std::vector <struct pollfd> fd_poll;
		
		struct pollfd fd_poll[10241];
		std::string servername;
		std::string hostname;
		std::string pass;
		sockaddr_in service;
		int number_of_clients;
		int port;
		int message_code;

		/*Capability Negotiation Settings for IRSSI SERVER SIDE*/
		bool multi_prefix;
		bool extended_join;
		bool away_notify;
		bool chghost;
		bool account_notify;
		bool server_time;
		std::string sasl;
		/*complex settings*/
		
		// bool tls;
		// bool account_tag;
		// bool cap_notify;
		// bool echo_message;

		/*complex settings*/

		/*Capability Negotiation Settings*/
    /* VARIABLES*/
	public:
	/*orth form*/
    	Server ();
		Server (std::string port,std::string pass);

    	Server (const Server &a);
    	~Server ();
		Server& operator=(const Server &a);
	/*orth Server*/
	/*exception*/
		int serverInit();
		int runServer();
		int	serverLoop();
		int	connectionEvent();
		int register_user(Client * user);
		int Recv_end(int fd, std::string& line);
		std::string msg(std::string source, std::string command, std::string param, std::string text);
		int         id_check(Client *user);
		std::string cap_ls();




	
	/*exception*/
	/*getters and setters*/
	/*getters and setters*/
	/*extra*/
	/*extra*/
};
#endif

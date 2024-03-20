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
#include <signal.h>
#include <vector>
#include <map>
#include <fcntl.h>
#include <cstring>
#include "../inc/Client.hpp"
#include "../inc/parsing.hpp"

/*
*/
// static int g_signal;
class Server
{
	/* VARIABLES*/
	private:
		int server;
		std::vector <Client*> client;
		std::vector <std::string> nicknames;
		std::vector <struct pollfd> fd_poll;
		int port;
		std::string ports;
		std::string pass;

		int number_of_clients;
		int message_code;
		sockaddr_in service;

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
		static void		sighandle(int sig);
		int			serverInit();
		int			runServer();
		int			serverLoop();
		int			connectionEvent();
		int			register_user(Client * user);
		int			Recv_end(int fd, std::string & line);
		void		commandPath(ircMessage msg, Client * user);
		std::string	msg(std::string source, std::string command, std::string param, std::string text);
		std::string cap_ls();
		std::string cap_ack(ircMessage cap_list);
	/*exception*/
	/*getters and setters*/
	/*getters and setters*/
	/*extra*/
	/*extra*/
};
#endif

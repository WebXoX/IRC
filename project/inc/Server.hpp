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
#include "Client.hpp"
#include "parsing.hpp"
#include "Channel.hpp"
#include "common.hpp"
#include "Reply.hpp"

#include <netdb.h>
/*
*/
// int server_status;
class Server
{
	/* VARIABLES*/
	private:
	/*   server socket variables  */
		int server;
		sockaddr_in service;
		int port;
		std::string ports;
		std::string server_name;
		std::vector <struct pollfd> fd_poll;
		int guestuser;
	/*   server socket variables  */
	/*   server info  */
		std::string pass;
		std::string creation_date;
		int number_of_clients;
		std::vector <Client*> client;
		std::vector <std::string> nicknames;
	/*   server info  */

	/*   future extras  */
	/*   future extras  */

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
		/* **********SERVER **********	*/
	
		int					serverInit();
		int					runServer();
		int					serverLoop();
		int					Recv_end(int fd, std::string & line);
		int 				definedmessage(int fd,std::string str);
		int					connectionEvent();
		/* **********SERVER **********	*/
		/* **********REGISTRATION COMMANDS**********	*/
		int					register_user(ircMessage msg, Client * user);
		void				commandPath(ircMessage msg, Client * user);
		void 				adduser(Client * user, ircMessage msg);
		void				nick(Client * user, std::string str);
		void 				callMotd(Client * user);
		int 				MOTD(Client * user);
		void		 		cap_ls(Client *user);
		void		 		cap_ack(Client *user);
		std::string 		date_now(char *buffer);
		/* **********REGISTRATION COMMANDS**********	*/
		void 				free_delete();
		
		std::string			msg(std::string source, std::string command, std::string param, std::string text);
		// std::map<std::string,std::string> modeParser(ircMessage msg);

		// ********** CHANNEL ********** //

		std::map<std::string, Channel>				channels;
		std::map<std::string, Channel>::iterator	chan_it;
		void 										quitCmd(std::string msg, Client * user);
		bool										validcommand(std::string cmd);
		bool 										isUser(Client& user);
		bool 										isUserNick(std::string nickname);
		int 										addChannelInServer( Channel& channel);
		bool 										hasChannelInServer(std::string channelName);

		// ********** COMMANDS ********** //

		void										modeCommand(ircMessage& msg, Client& user);
		int 										modechange(std::map<std::string,std::string> mode, Channel& channel);
		void 										joinCommand(ircMessage& msg,  Client& user);
		void 										topicCommand(ircMessage msg, Client& user);
		void 										kickCommand(ircMessage msg, Client& user);
		void 										inviteCommand(ircMessage msg, Client& user);
		void 										privmsgCommand(ircMessage msg, Client& user);
	/*exception*/
	/*getters and setters*/
		Client*										getClient(std::string nickname);
		std::map<std::string, Channel>::iterator	getChannelIt(std::string chanName);
		Client*										getUser(std::string name);
		int 										getuser_fd(std::string name);
	/*getters and setters*/
	/*extra*/
	/*extra*/
};



#endif

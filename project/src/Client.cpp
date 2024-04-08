#include "../inc/Client.hpp"

/* orth Client / constructor*/
Client::Client ()
{
	this->client_fd = 0;
	// this->nickname =;
	this->regi_status = 0;
	memset(this->buffer,0,1024);
	// std::cout << "Client default constructor" << std::endl;
}

Client::Client (const Client &a)
{
	std::cout << "Client copy constructor " << std::endl;
		this->client_fd = a.client_fd;
		this->regi_status = a.regi_status;
		this->hostname = a.hostname; // Real name/address of the host
		this->ip = a.ip; // Real name/address of the host
		this->username = a.username;  // Username of the client on the host
		this->realname = a.realname;  // Realname of the client on the host
		this->nickname = a.nickname;   // Nickname chosen by the client
		this->server = a.server;   // Server to which the client is connected
		this->channel = a.channel;   // channel to which the client is connected
	memset(this->buffer,0,1024);
		this->port = a.port;
		/*Capability Negotiation Settings for IRSSI CLIENT SIDE*/
		this->multi_prefix = a.multi_prefix;
		this->extended_join = a.extended_join;
		this->away_notify = a.away_notify;
		this->chghost = a.chghost;
		this->account_notify = a.account_notify;
		this->server_time = a.server_time;

}

Client::~Client ()
{
	// std::cout << "Client distructor called" << std::endl;
}

Client& Client::operator=(const Client& rhs)
{
    if (this != &rhs)
    {
        // std::cout << "Client copy assignment operator = " << std::endl;
		this->client_fd = rhs.client_fd;
		this->regi_status = rhs.regi_status;
		this->hostname = rhs.hostname; // Real name/address of the host
		this->ip = rhs.ip; // Real name/address of the host
		this->username = rhs.username;  // Username of the client on the host
		this->realname = rhs.realname;  // Realname of the client on the host
		this->nickname = rhs.nickname;   // Nickname chosen by the client
		this->server = rhs.server;   // Server to which the client is connected
		this->channel = rhs.channel;   // channel to which the client is connected
		memset(this->buffer,0,1024);
		this->port = rhs.port;
		/*Capability Negotiation Settings for IRSSI CLIENT SIDE*/
		this->multi_prefix = rhs.multi_prefix;
		this->extended_join = rhs.extended_join;
		this->away_notify = rhs.away_notify;
		this->chghost = rhs.chghost;
		this->account_notify = rhs.account_notify;
		this->server_time = rhs.server_time;
    }
    return *this;
}
/* orth Client */
/*extra*/

/*extra*/
/*getter and setters*/
/*getter and setters*/


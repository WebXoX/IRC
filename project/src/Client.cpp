#include "../inc/Client.hpp"

/* orth Client / constructor*/
Client::Client ():nickname(),username(),realname(),hostname(),server(),channel(),port()
{
	this->client_fd = 0;
	this->pass_status = 0;
	memset(this->buffer,0,1024);
	// std::cout << "Client default constructor" << std::endl;
}

Client::Client (const Client &a)
{
	std::cout << "Client copy constructor " << std::endl;
		this->client_fd = a.client_fd;
		this->hostname = a.hostname; // Real name/address of the host
		this->username = a.username;  // Username of the client on the host
		this->realname = a.realname;  // Realname of the client on the host
		this->nickname = a.nickname;   // Nickname chosen by the client
		this->server = a.server;   // Server to which the client is connected
		this->channel = a.channel;   // channel to which the client is connected
		memset(this->buffer,0,1024);
		this->port = a.port;
		/*Capability Negotiation Settings for IRSSI CLIENT SIDE*/

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
		this->hostname = rhs.hostname; // Real name/address of the host
		this->username = rhs.username;  // Username of the client on the host
		this->realname = rhs.realname;  // Realname of the client on the host
		this->nickname = rhs.nickname;   // Nickname chosen by the client
		this->server = rhs.server;   // Server to which the client is connected
		this->channel = rhs.channel;   // channel to which the client is connected
		memset(this->buffer,0,1024);
		this->port = rhs.port;
    }
    return *this;
}
/* orth Client */
/*extra*/
	bool Client::registerstatus()
	{
		if(this->pass_status == 1 && this->username.empty() == false && this->realname.empty() == false && this->nickname.empty() == false )
			return true;
		return false;
	}

/*extra*/
/*getter and setters*/
/*getter and setters*/


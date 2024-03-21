#include "../inc/Client.hpp"

/* orth Client / constructor*/
Client::Client ()
{
	this->client_fd = 0;
	this->regi_status = 0;
	memset(this->buffer,0,1024);
	// std::cout << "Client default constructor" << std::endl;
}

Client::Client (const Client &a)
{
	// std::cout << "Client copy constructor " << std::endl;
	*this = a;
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
		*this = rhs;
    }
    return *this;
}
/* orth Client */
/*extra*/

/*extra*/
/*getter and setters*/
/*getter and setters*/

#include "../inc/Server.hpp"
#include "../inc/Client.hpp"

int main (int argc, char *argv[])
{
	if(argc == 3)
	{
		std::string port = argv[1];
		std::string pass = argv[2];
		if(port.find_first_not_of("0123456789") != std::string::npos 
			&& atoi(port.c_str()) >= 1024 && atoi(port.c_str()) <= 65535)
		{
			std::cerr << "Invalid port Range( 0 - 65535)" << std::endl;
			return 1;
		}
		if(pass.empty())
		{
			std::cerr << "Password cannot be empty" << std::endl;
			return 1;
		}
		Server server = Server(port,pass);
		if(server.serverInit() == 0)
		{
			server.runServer();
			server.serverLoop();
		}
	}
	else
	{
		std::cerr << "Usage: " << argv[0] << " <port>" << " <password>"<< std::endl;
	}
	return 0;
}
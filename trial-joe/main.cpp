#include "Server.hpp"
int main ()
{
	Server server;
	if(server.serverInit() == 0)
	{
		server.runServer();
		server.serverLoop();
	}
	
	return 0;
}
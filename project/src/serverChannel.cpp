#include "../inc/Server.hpp"


bool Server::hasChannel(std::string channelName) {
	return this->channels.find(channelName) != this->channels.end();
}
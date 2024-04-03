#include "../inc/Channel.hpp"
#include "../inc/Client.hpp"
#include "../inc/Server.hpp"
#include "../inc/parsing.hpp"


Channel::Channel(const std::string& name,  Client& user): name(name) {
    this->addUserInChannel(user);
}

Channel::~Channel() {}

int Channel::addUserInChannel( Client& user) {
    if (this->isUserInChannel(user)) {
        return 1;
    }
    this->users[user.client_fd] = &user;
    return 0;
}

bool Channel::isUserInChannel( Client& user) {
    return this->users.find(user.client_fd) != this->users.end();   
}



// ****** GETTERS ****** //

std::string Channel::getName()  {
    return this->name;
}






// ****** SERVER ****** //

std::string Server::joinCommand(ircMessage msg, Client& user) {
    (void)user;
    std::string chanName = msg.params[0];
    if (this->hasChannel(chanName))
        return "Channel already exists";
    Channel newChannel(chanName, user);
    this->addChannel(newChannel);
    // add channel to the client
    return "Channel created AND ADDED";
}

int Server::addChannel( Channel& channel) {
    if (!this->hasChannel(channel.getName())) {
        this->channels[channel.getName()] = &channel;
        return 1;
    }
    return 0;
}

bool Server::hasChannel(std::string channelName) {
	return this->channels.find(channelName) != this->channels.end();
}


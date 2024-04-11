#include "../inc/Server.hpp"

void Server::joinCommand(ircMessage msg, Client& user) {
    std::string errMsg;

    // check it later potencial problem
    if (msg.params.size() < 1) {
        errMsg = ERR_NEEDMOREPARAMS(user.username, "JOIN");
        return;
    }
    
    Channel::validate_channels(msg.params);
    printCommand(msg);

    for (size_t i = 0; i < msg.params.size(); i++) {
        if (hasChannelInServer(msg.params[i])) 
            this->channels[msg.params[i]].addUser(user);
        else {
            Channel newChannel(msg.params[i], user);
            this->addChannelInServer(newChannel);
        }
    }
}
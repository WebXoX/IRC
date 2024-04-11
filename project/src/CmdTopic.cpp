#include "../inc/Server.hpp"

void Server::topicCommand(ircMessage msg, Client& user) {
    // TOPIC #tehuan_world :new topic message
    this->channels[msg.params[0]].setTopic(msg.trailing, user);

}

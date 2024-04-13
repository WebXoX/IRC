#include "../inc/Server.hpp"

void Server::privmsgCommand(ircMessage msg, Client& user) {
    (void)user;
    (void)msg;
    
    // TOPIC #tehuan_world :new topic message
    // std::string message = "";
    // std::string topic = msg.trailing;
    // std::string chanName = msg.params[0];

    // if (msg.params.size() != 1) {
    //     message = ERR_NEEDMOREPARAMS(user.nickname, "TOPIC");
    //     send(user.client_fd, message.c_str(), message.size(), 0);
    //     return;
    // } else if (msg.trailing.empty()) {
    //     if (this->channels[chanName].hasTopic())
    //         message = RPL_TOPIC(user.nickname, chanName, this->channels[chanName].getTopic());
    //     else
    //         message = RPL_NOTOPIC(user.nickname, chanName);
    // } else {

    //     if (!this->channels[chanName].isUser(user)) 
    //         message = ERR_NOTONCHANNEL(user.nickname, chanName);
    //     else if (this->channels[chanName].isModeSet('t') && !this->channels[chanName].isOperator(user)) 
    //         message = ERR_CHANOPRIVSNEEDED(user.nickname, chanName);
    //     else {
    //         this->channels[chanName].setTopic(topic);
    //         message = RPL_TOPIC(user.nickname, chanName, topic);
    //     }

    // }

    // send(user.client_fd, message.c_str(), message.size(), 0);
    

}

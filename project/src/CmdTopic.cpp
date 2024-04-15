#include "../inc/Server.hpp"

void Server::topicCommand(ircMessage msg, Client& user) {
    // TOPIC #tehuan_world :new topic message
    std::string reply = "";
    std::string topic = msg.trailing;
    std::string chanName = msg.params[0];


    std::cout << "inside topic function" << std::endl;
    if (msg.params.size() != 1) {
        reply = ERR_NEEDMOREPARAMS(user.nickname, "TOPIC");
    } 
    else if (!this->hasChannelInServer(chanName)) {
        reply = ERR_NOSUCHCHANNEL(user.nickname, chanName);
    }
    else if (!this->channels[chanName].isUser(user)) {
        reply = ERR_NOTONCHANNEL(user.nickname, chanName);
    }
    else if (msg.trailing.empty()) {
        if (this->channels[chanName].hasTopic())
            reply = RPL_TOPIC(user.nickname, chanName, this->channels[chanName].getTopic());
        else
            reply = RPL_NOTOPIC(user.nickname, chanName);
    } 
    else {
        if (this->channels[chanName].isModeSet('t') && !this->channels[chanName].isOperator(user)) 
            reply = ERR_CHANOPRIVSNEEDED(user.nickname, chanName);
        else {
            this->channels[chanName].setTopic(topic);
            reply = RPL_CHANGETOPIC(user_id(user.nickname,user.username), chanName, topic);
            this->channels[chanName].broadcast(reply);
            return;
        }
    }
this->definedmessage(user.client_fd, reply);
}

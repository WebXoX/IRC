#include "../inc/Server.hpp"


void Server::kickCommand(ircMessage msg, Client& user) {
    std::string message = "";
    if (msg.params.size() != 2) 
        message = ERR_NEEDMOREPARAMS(user.nickname, "KICK");

    std::string chanName = msg.params[0];
    std::string kicker = user.nickname;
    std::string kickedName = msg.params[1];
    std::string reason = msg.trailing;
    
    
    // nc does not split the nicknames case tested on nc
    // in this case we need to split the nicknames into a vector
    std::vector<std::string> kickedList;
    if (kickedName.find(',') != std::string::npos)
        kickedList = Channel::split(kickedName, ',');
    else
        kickedList.push_back(kickedName);

    if (!hasChannelInServer(chanName)) {
        message = ERR_NOSUCHCHANNEL(kicker, chanName);
    } else if (!this->channels[chanName].isOperator(user)) {
        message = ERR_CHANOPRIVSNEEDED(kicker, chanName);
    } else {
        for (size_t i = 0; i < kickedList.size(); i++) {

            Client newClient;
            Client& kickedClient = newClient;

            for (size_t j = 0; j < this->client.size(); j++) {
                if (this->client[j]->nickname == kickedList[i])
                    kickedClient = *this->client[j];
            }

            if (this->channels[chanName].isUser(kickedClient)) {
                this->channels[chanName].removeUser(kickedClient);
                if (reason.empty())
                    reason = kickedList[i];
                message = RPL_KICK(user_id(user.nickname, user.username), chanName, kickedList[i], reason);
            } else {
                message = ERR_NOSUCHNICK(kicker, kickedList[i]);
            }
            if (this->channels[chanName].isOperator(kickedClient))
                this->channels[chanName].removeOperator(kickedClient);

            send(user.client_fd, message.c_str(), message.size(), 0);
        }
        return;
    }
    send(user.client_fd, message.c_str(), message.size(), 0);
}
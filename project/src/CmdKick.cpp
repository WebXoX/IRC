#include "../inc/Server.hpp"


void Server::kickCommand(ircMessage msg, Client& user) {
    std::string message = "";
    if (msg.params.size() != 2) 
        message = ERR_NEEDMOREPARAMS(user.nickname, "KICK");

    std::string chanName = msg.params[0];
    std::string kicker = user.nickname;
    std::string kickedName = msg.params[1];
    std::string reason = msg.trailing;
    
    std::vector<std::string> kickedList; // nc does not split the nicknames. In this case we need to split the nicknames into a vector
    if (kickedName.find(',') != std::string::npos)
        kickedList = split(kickedName, ',');
    else
        kickedList.push_back(kickedName);

    if (!hasChannelInServer(chanName)) {
        message = ERR_NOSUCHCHANNEL(kicker, chanName);
    } else if (!this->channels[chanName].isOperator(user)) {
        message = ERR_CHANOPRIVSNEEDED(kicker, chanName);
    } else {
        for (size_t i = 0; i < kickedList.size(); i++) {
            // As the client list is a vector of pointers thats the way to get a client reference
            Client newClient;
            Client& kickedClient = newClient;
            for (size_t j = 0; j < this->client.size(); j++) {
                if (this->client[j]->nickname == kickedList[i])
                    kickedClient = *this->client[j];
            }

            if (this->channels[chanName].isUser(kickedClient)) { // check if the user to be kicked is in the channel
                if (reason.empty()) // if no reason given set it to the kicked username
                    reason = kickedList[i];
                message = RPL_KICK(user_id(user.nickname, user.username), chanName, kickedList[i], reason);
                this->channels[chanName].broadcast(message);
                this->channels[chanName].removeFromAll(kickedClient); // remove the user from all lists in the channel
            } 
            else { // if the user is not in the channel return this message
                message = ERR_NOSUCHNICK(kicker, kickedList[i]);
                this->definedmessage(user.client_fd, message);
            }
        }
        return;
    }
    this->definedmessage(user.client_fd, message);

}
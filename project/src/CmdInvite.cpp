#include "../inc/Server.hpp"


void Server::inviteCommand(ircMessage msg, Client& user) {
    std::string reply = "";
    std::string invitedNick = msg.params[0];
    std::string chanName = msg.params[1];

    this->chan_it = this->getChannelIt(chanName);

    Client newClient;
    Client& invitedClient = newClient;
    for (size_t i = 0; i < this->client.size(); i++) {
        if (this->client[i]->nickname == invitedNick)
            invitedClient = *this->client[i];
    }

    if (msg.params.size() != 2) {
        reply = ERR_NEEDMOREPARAMS(user.nickname, "INVITE");
    } else if (!this->hasChannelInServer(chanName) || this->chan_it->second.howManyUsers() == 0) {
        reply = ERR_NOSUCHCHANNEL(user.nickname, chanName); 
    } else if (!this->isUserNick(invitedNick)) {
        reply = ERR_NOSUCHNICK(user.nickname, invitedNick);
    } else if (!this->chan_it->second.isUser(user)) {
        reply = ERR_NOTONCHANNEL(user.nickname, chanName);
    } else if (this->chan_it->second.isModeSet('i') && !this->chan_it->second.isOperator(user)) {
        reply = ERR_CHANOPRIVSNEEDED(user.nickname, chanName);
    } else if (this->chan_it->second.isUser(invitedClient)) {
        reply = ERR_USERONCHANNEL(user.nickname, invitedNick, chanName);
    } else {
        reply = RPL_INVITE(user_id(user.nickname, user.username), invitedNick, chanName);
        send(invitedClient.client_fd, reply.c_str(), reply.size(), 0);
        reply = RPL_INVITING(user_id(user.nickname, user.username), user.nickname, invitedNick, chanName);
        send(user.client_fd, reply.c_str(), reply.size(), 0);
        this->chan_it->second.addInvited(invitedClient);
        return;
    }
    send(user.client_fd, reply.c_str(), reply.size(), 0);
      
}
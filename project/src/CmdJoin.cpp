#include "../inc/Server.hpp"
#include "../inc/Reply.hpp"

#define MSG_JOIN(nickname, channel) (":" + nickname + " JOIN " + channel + "\r\n")

void Server::joinCommand(ircMessage msg, Client& user) {
    std::string reply = "";

    // check it later potencial problem
    if (msg.params.size() < 1) { reply = ERR_NEEDMOREPARAMS(user.nickname, "JOIN"); return; }
    Channel::splitChannelsName(msg.params);
    printCommand(msg);
    std::vector<std::string> chans = msg.params;

    for (size_t i = 0; i < chans.size(); i++) {
        if (hasChannelInServer(chans[i])) {
            if (this->channels[chans[i]].isUser(user)) 
                reply = ERR_USERONCHANNEL(user.username, user.nickname, chans[i]);
            else if (this->channels[chans[i]].isModeSet('l') && this->channels[chans[i]].howManyUsers() >= this->channels[chans[i]].getUserLimit()) 
                reply = ERR_CHANNELISFULL(user.hostname, chans[i]);
            else if (this->channels[chans[i]].isModeSet('i') && !this->channels[chans[i]].isInvited(user)) 
                reply = ERR_INVITEONLYCHAN(user.nickname, chans[i]);
            else {
                reply += RPL_JOIN(user_id(user.nickname, user.username), chans[i]);
                this->channels[chans[i]].broadcast(reply);
                if (this->channels[chans[i]].hasTopic()) 
                    reply += RPL_TOPIC(user.nickname, chans[i], this->channels[chans[i]].getTopic());
                this->channels[chans[i]].addUser(user);
                reply += RPL_NAMREPLY(user.nickname, '@', chans[i], this->channels[chans[i]].getListOfUsers());
                reply += RPL_ENDOFNAMES(user.nickname, chans[i]);
            }
        }
        else {
            Channel newChannel(msg.params[i], user);
            this->addChannelInServer(newChannel);
            reply += RPL_JOIN(user_id(user.nickname, user.username), chans[i]);
            reply += RPL_NAMREPLY(user.nickname, '@', chans[i], this->channels[chans[i]].getListOfUsers());
            reply += RPL_ENDOFNAMES(user.nickname, chans[i]);
        }
        send(user.client_fd, reply.c_str(), reply.size(), 0);
    }
}


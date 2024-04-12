#include "../inc/Server.hpp"
#include "../inc/Reply.hpp"

#define MSG_JOIN(nickname, channel) (":" + nickname + " JOIN " + channel + "\r\n")

std::vector<std::pair<std::string, std::string> > createChannelKeyMap(std::vector<std::string> args) {

    std::vector<std::pair<std::string, std::string> > channelKeyList;
    std::vector<std::string> channelList;
    std::vector<std::string> keyList;

    for (size_t i = 0; i < args.size(); i++) {
        if (i == 0)
            channelList = split(args[i], ',');
        else if (i == 1)
            keyList = split(args[i], ',');
    }


    for (std::size_t i = 0; i < channelList.size(); ++i) {
        if (i < keyList.size()) {
            channelKeyList.push_back(std::make_pair(channelList[i], keyList[i]));
        } else {
            channelKeyList.push_back(std::make_pair(channelList[i], ""));
        }
    }

    return channelKeyList;
}

void Server::joinCommand(ircMessage msg, Client& user) {
    (void)user;
    std::string reply = "";

    // check if we have at least on parameter a channel
    if (msg.params.size() > 2) { return; }

    // creates a map in wich channel name as the key and password(key) as the value
    std::vector<std::pair<std::string, std::string> > channelKeyList = createChannelKeyMap(msg.params);

    for (size_t i = 0; i < channelKeyList.size(); ++i) {
        std::cout << channelKeyList[i].first << " => " << channelKeyList[i].second << std::endl;
    }


    printCommand(msg);

    // std::vector<std::string> chans = msg.params;

    // for (size_t i = 0; i < chans.size(); i++) {
    //     if (hasChannelInServer(chans[i])) {
    //         if (this->channels[chans[i]].isUser(user)) 
    //             reply = ERR_USERONCHANNEL(user.username, user.nickname, chans[i]);
    //         else if (this->channels[chans[i]].isModeSet('l') && this->channels[chans[i]].howManyUsers() >= this->channels[chans[i]].getUserLimit()) 
    //             reply = ERR_CHANNELISFULL(user.hostname, chans[i]);
    //         else if (this->channels[chans[i]].isModeSet('i') && !this->channels[chans[i]].isInvited(user)) 
    //             reply = ERR_INVITEONLYCHAN(user.nickname, chans[i]);
    //         else {
    //             reply += RPL_JOIN(user_id(user.nickname, user.username), chans[i]);
    //             this->channels[chans[i]].broadcast(reply);
    //             if (this->channels[chans[i]].hasTopic()) 
    //                 reply += RPL_TOPIC(user.nickname, chans[i], this->channels[chans[i]].getTopic());
    //             this->channels[chans[i]].addUser(user);
    //             reply += RPL_NAMREPLY(user.nickname, '@', chans[i], this->channels[chans[i]].getListOfUsers());
    //             reply += RPL_ENDOFNAMES(user.nickname, chans[i]);
    //         }
    //     }
    //     else {
    //         Channel newChannel(msg.params[i], user);
    //         this->addChannelInServer(newChannel);
    //         reply += RPL_JOIN(user_id(user.nickname, user.username), chans[i]);
    //         reply += RPL_NAMREPLY(user.nickname, '@', chans[i], this->channels[chans[i]].getListOfUsers());
    //         reply += RPL_ENDOFNAMES(user.nickname, chans[i]);
    //     }
    //     send(user.client_fd, reply.c_str(), reply.size(), 0);
    // }
}


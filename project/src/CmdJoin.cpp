#include "../inc/Server.hpp"
#include "../inc/Reply.hpp"

// conditions to check in order to join a channel
    // channel exists? (yes)
    //     is already inside the channel?
    //         send error return
    //     has modes enabled?
    //         has limits enabled 'l'?
    //             channel is full?
    //                 send error and return
    //         has invite only enabled'i'?
    //             is user in the invites lists? (no)
    //                 send error and return
    //         has password enabled'k'? (yes)
    //             check the password: (wrong)
    //                 send error and return
    //     broadcast join reply
    //     add user in the channel
    // channel exists? (no)
    //     create channel
    //     add user in
    //     set user as operator
    //     add channel into the server
    // send welcome to the user

std::vector<std::pair<std::string, std::string> > createChannelKeyMap(std::vector<std::string> args) {
    std::vector<std::pair<std::string, std::string> > channelKeyList; // a vector of pairs to store the channel and its password, a vector was choosen because map sorts the insertion and we want to keep the order
    std::vector<std::string> channelList; // list to store the channels
    std::vector<std::string> keyList; // list to store the passwords
    for (size_t i = 0; i < args.size(); i++) {
        if (i == 0) // makes sure the first argument will be the channels
            channelList = split(args[i], ','); // if theres a comma split the arguments
        else if (i == 1) // makes sure the second argument is the passwords
            keyList = split(args[i], ','); /// if comma found split 
    }
    for (std::size_t i = 0; i < channelList.size(); ++i) { // iterates over the channels list
        if (i < keyList.size()) // if there is a password on the password list 
            channelKeyList.push_back(std::make_pair(channelList[i], keyList[i])); // make a pair and store them 
        else 
            channelKeyList.push_back(std::make_pair(channelList[i], "")); // otherwise give a empty string to the pass and store both
    }
    return channelKeyList;
}

std::string welcomeMessage(Client& user, Channel& channel) {
    std::string reply;

    reply = RPL_JOIN(user_id(user.nickname, user.username), channel.getName());
    if (channel.howManyUsers() == 1)
        reply += MODE_CHANNELMSG(channel.getName(), channel.getModes());
    if (channel.hasTopic()) // if has a topic append it to the message
        reply += RPL_TOPIC(user.nickname, channel.getName(), channel.getTopic());
    reply += RPL_NAMREPLY(user.nickname, '@', channel.getName(), channel.getListOfUsers());
    reply += RPL_ENDOFNAMES(user.username, channel.getName());

    return reply;
}


void Server::joinCommand(ircMessage& msg, Client& user) {

    std::string reply = "";
    if (msg.params.size() > 2) { return; }  // check if we have only two parameters channels name and passwords

    // creates a map in wich channel name as the key and password as the value
    std::vector<std::pair<std::string, std::string> > channelKeyList = createChannelKeyMap(msg.params);
   
    for (size_t i = 0; i < channelKeyList.size(); ++i) {  // loops the list of channels
        std::string chanName = channelKeyList[i].first; // the channel name to be searched
        if (this->hasChannelInServer(chanName)) { // channel exists in the server (yes)
            this->chan_it = this->getChannelIt(chanName); // gets an iterator to the current channel
            Channel channel = chan_it->second; // create a copy of the real channel just to be easy to read
            if (channel.isUser(user)) // is already inside the channel?
                reply = ERR_USERONCHANNEL(user.username, user.nickname, chanName);
            else if (!channel.isInvited(user) && channel.isModeSet('l') && channel.howManyUsers() >= channel.getUserLimit()) // has mode limit seted? (yes) check if the channel is full
                reply = ERR_CHANNELISFULL(user.nickname, chanName);
            else if (channel.isModeSet('i') && !channel.isInvited(user)) // is invite only seted? (yes) Is the user in the invite list
                reply = ERR_INVITEONLYCHAN(user.nickname, chanName);
            else if (channel.isModeSet('k') && channelKeyList[i].second != channel.getPassword()) // is password mode seted? (yes) check if the password match
                reply = ERR_BADCHANNELKEY(user.nickname, chanName);
            else {
                reply = RPL_JOIN(user_id(user.nickname, user.username), chanName);
                chan_it->second.broadcast(reply); // cant use channel variable here cause will add the user into the copy
                chan_it->second.addUser(user);
                reply = welcomeMessage(user, chan_it->second);
            }
                user.toSend.push_back(reply);
            return;
        }
        else { // channel exists in the server (no)
            Channel newChannel(chanName, user); // this contructor already add the user and set as operator
            this->channels[chanName] = newChannel; // create a copy into the server. At the creation time is okay
        }   

        // send the welcome message to the user who requested the join
        reply = welcomeMessage(user, this->getChannelIt(chanName)->second);
        user.toSend.push_back( reply);
        std::cout << "server ==>> "<< reply << std::endl;
    }

}


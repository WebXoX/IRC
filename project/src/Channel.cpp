#include "../inc/Channel.hpp"
#include "../inc/Reply.hpp"


Channel::Channel() {}

Channel::Channel(std::string& name, Client& user): name(name) {
    this->addUserInChannel(user);
    this->setChannelOperator(user);
}

Channel::Channel(const Channel& copy) { *this = copy; }

Channel& Channel::operator=(const Channel& copy) {
    this->name = copy.name;
    std::map<int, Client*>::const_iterator it;
    for (it = copy.users.begin(); it != copy.users.end(); it++)
        this->users[it->first] = it->second;
    for (it = copy.operators.begin(); it != copy.operators.end(); it++)
        this->operators[it->first] = it->second;
    return *this;
}

Channel::~Channel() {}

std::string Channel::listOfUsers() {
    std::string listusers = "";
    std::map<int, Client*>::iterator it = users.begin();
    for (; it != users.end(); it++) {
        listusers += it->second->nickname + " ";
    }
    return listusers;
}


std::string Channel::welcomeMessage(Client& user) {
    return RPL_JOIN(user_id(user.nickname, user.username), this->name) +
        RPL_NAMREPLY(user.username, "-", this->name, this->listOfUsers()) +
        RPL_ENDOFNAMES(user.username, this->name);
}

void Channel::announceNewUser(Client& user) {
    std::string message = RPL_JOIN(user_id(user.nickname, user.username), this->name);
    std::map<int, Client*>::iterator it;
    for (it = this->users.begin(); it != this->users.end(); it++) {
        if (it->second->currentChannel == this->name)
            send(it->first, message.c_str(), message.length(), 0);
    }
}

std::string Channel::addUserInChannel(Client& user) {
    if (this->isUserInChannel(user)) {
        if (user.currentChannel != this->name) {
            user.currentChannel = this->name;
            this->announceNewUser(user);
            return "";
        }
        return ERR_USERONCHANNEL(user.username, user.nickname, this->name);
    }
    this->announceNewUser(user);
    this->users[user.client_fd] = &user;
    user.currentChannel = this->name;
    return this->welcomeMessage(user);
}

std::string Channel::setChannelOperator(Client& user) {
    if (!this->isUserInChannel(user))
        return ERR_NOTONCHANNEL(user.username, this->name);
    this->operators[user.client_fd] = &user;
    return RPL_YOUREOPER(user.username);
}

void Channel::broadcastMessage(Client& user, std::string message) {
    std::string newMessage = "<@" + user.nickname + "> " + message;
    std::map<int, Client*>::iterator it;
    for (it = this->users.begin(); it != this->users.end(); it++) {
        if (it->second->currentChannel == this->name)
            send(it->first, newMessage.c_str(), newMessage.length(), 0);
    }

}

// // ****** CHECKERS ****** //
bool Channel::isUserInChannel(Client& user) {
    return this->users.find(user.client_fd) != this->users.end();
}

bool Channel::isUserOperator(Client& user) {
    return this->operators.find(user.client_fd) != this->operators.end();
}

int Channel::howManyUsersInChannel() {
    return this->users.size();
}






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
    std::map<int, Client>::const_iterator it;
    for (it = copy.users.begin(); it != copy.users.end(); it++)
        this->users[it->first] = it->second;
    for (it = copy.operators.begin(); it != copy.operators.end(); it++)
        this->operators[it->first] = it->second;
    return *this;
}

Channel::~Channel() {}


std::string Channel::addUserInChannel(Client& user) {
    if (this->isUserInChannel(user))
        return ERR_USERONCHANNEL(user.username, user.nickname, this->name);
    this->users[user.client_fd] = user;
    user.currentChannel = this->name;
    return RPL_JOIN(user_id(user.nickname, user.username), this->name);
}

std::string Channel::setChannelOperator(Client& user) {
    if (!this->isUserInChannel(user))
        return ERR_NOTONCHANNEL(user.username, this->name);
    this->operators[user.client_fd] = user;
    return RPL_YOUREOPER(user.username);
}

// // ****** CHECKERS ****** //
bool Channel::isUserInChannel(Client& user) {
    return this->users.find(user.client_fd) != this->users.end();
}

bool Channel::isUserOperator(Client& user) {
    return this->operators.find(user.client_fd) != this->operators.end();
}






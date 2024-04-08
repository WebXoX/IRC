#include "../inc/Channel.hpp"
#include "../inc/Reply.hpp"


Channel::Channel() {}

Channel::Channel(std::string& name) {
    this->name = name;
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

std::string Channel::addUserInChannel(Client& user) {
    if (this->isUserInChannel(user)) 
        return ERR_USERONCHANNEL(user.hostname, user.nickname, this->name);

    this->users[user.client_fd] = &user;
    this->topic = "Welcome to the channel " + this->name;
    return RPL_JOIN(user_id(user.nickname, user.username), this->name) +
            RPL_TOPIC(user.hostname, this->name, this->topic) +
            RPL_NAMREPLY(user.username, "-", this->name, this->listOfUsers()) +
            RPL_ENDOFNAMES(user.hostname, this->name);
}

std::string Channel::setChannelOperator(Client& user) {
    if (!this->isUserInChannel(user))
        return ERR_NOTONCHANNEL(user.hostname, this->name);
    this->operators[user.client_fd] = &user;
    return RPL_YOUREOPER(user.hostname);
}

void Channel::broadcastMessage(std::string message) {
    std::map<int, Client*>::iterator it;
    for (it = this->users.begin(); it != this->users.end(); it++) 
            send(it->first, message.c_str(), message.length(), 0);
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

bool Channel::hasChannelTopic() {
    return this->topic != "";
}

std::string Channel::setChannelTopic(Client& user, std::string topic) {
    if (!this->isUserInChannel(user))
        return ERR_USERNOTINCHANNEL(user.hostname, user.nickname, this->name);
    if (!this->isUserOperator(user))
        return ERR_NOPRIVILEGES(user.hostname);
    this->topic = topic;
    return RPL_TOPIC(user.hostname, this->name, this->topic);
}






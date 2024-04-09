#include "../inc/Channel.hpp"
#include "../inc/Reply.hpp"


Channel::Channel() {}

Channel::Channel(std::string& name) {
    this->name = name;
	this->topic = "";
	this->password = "";
	this->userLimit = 0;
	this->modes['i'] = 0;
	this->modes['t'] = 0;
	this->modes['k'] = 0;
	this->modes['o'] = 0;
	this->modes['l'] = 0;
}

Channel::Channel(std::string& name, Client& user) {
    this->name = name;
	this->topic = "";
	this->password = "";
	this->userLimit = 1;
	this->modes['i'] = 0;
	this->modes['t'] = 0;
	this->modes['k'] = 0;
	this->modes['o'] = 0;
	this->modes['l'] = 0;
    this->addUser(user);
    this->addOperator(user);
}

Channel::Channel(const Channel& copy) { *this = copy; }

Channel& Channel::operator=(const Channel& copy) {
    this->name = copy.name;
    this->topic = copy.topic;
    this->password = copy.password;
    this->userLimit = copy.userLimit;
    this->users = copy.users;
    this->operators = copy.operators;
    this->modes = copy.modes;
    return *this;
}

Channel::~Channel() {}

void Channel::addUser(Client& user) {
    this->users[user.client_fd] = &user;
    this->userLimit++;
}

void Channel::removeUser(Client& user) {
    this->users.erase(user.client_fd);
}

void Channel::addOperator(Client& user) {
    this->operators[user.client_fd] = &user;
}

void Channel::removeOperator(Client& user) {
    this->operators.erase(user.client_fd);
}

void Channel::broadcast(std::string message) {
    std::map<int, Client*>::iterator it = users.begin();
    for (; it != users.end(); it++) 
            send(it->first, message.c_str(), message.size(), 0);
}

/////   GETTERS     /////

std::string Channel::getName() { return this->name; }

std::string Channel::getTopic() { return this->topic; }

std::string Channel::getPassword() { return this->password; }

int Channel::getUserLimit() { return this->userLimit; }

std::map<int, Client*> Channel::getUsers() { return this->users; }

std::map<int, Client*> Channel::getOperators() { return this->operators; }

std::map<char, bool> Channel::getModes() { return this->modes; }

// returns a string with the nicknames of all users in the channel
std::string Channel::getListOfUsers() {
    std::string listusers = "";
    std::map<int, Client*>::iterator it = users.begin();
    for (; it != users.end(); it++) {
        listusers += it->second->nickname + " ";
    }
    return listusers;
}

/////   SETTERS     /////

void Channel::setName(std::string name) { this->name = name; }

void Channel::setTopic(std::string topic) { this->topic = topic; }

void Channel::setPassword(std::string password) { this->password = password; }

void Channel::setUserLimit(int userLimit) { this->userLimit = userLimit; }

void Channel::setMode(char mode, bool value) { this->modes[mode] = value; }


/////   CHECKERS    //////

int Channel::howManyUsers() { return this->users.size(); }

bool Channel::isUserChannel(Client& user) { return this->users.find(user.client_fd) != this->users.end(); }

bool Channel::isOperator(Client& user) { return this->operators.find(user.client_fd) != this->operators.end(); }

bool Channel::hasTopic() { return this->topic != ""; }

bool Channel::isMode(char mode) { return this->modes[mode]; }






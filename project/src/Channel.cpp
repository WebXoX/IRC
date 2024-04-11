#include "../inc/Channel.hpp"
#include "../inc/Reply.hpp"



Channel::Channel() {
    this->name = "Default";
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
	this->userLimit = 0;
	this->modes['i'] = 0;
	this->modes['t'] = 1;
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
    std::string message;
    if (this->isUser(user))
        message = ERR_USERONCHANNEL(user.hostname, user.nickname, this->name);
    else if (this->modes['l'] && this->howManyUsers() >= this->userLimit) {
        message = ERR_CHANNELISFULL(user.hostname, this->name);
    } else if (this->modes['i']) {

    } else {
        this->users[user.client_fd] = &user;
        if (this->howManyUsers() == 1)
            this->addOperator(user);
        message = RPL_JOIN(user_id(user.nickname, user.username), this->name);
        if (this->hasTopic())
            message += RPL_TOPIC(user.nickname, this->name, this->topic);
        message += RPL_NAMREPLY(user.nickname, "@", this->name, this->getListOfUsers());
        message += RPL_ENDOFNAMES(user.nickname, this->name);
    }
    std::cout << message << std::endl;
    int ret = send(user.client_fd, message.c_str(), message.size(), 0);
    if (ret == -1)
        perror("Error send: ");
}

void Channel::addOperator(Client& user) {
    this->operators[user.client_fd] = &user;
}

void Channel::addInvited(Client& user) {
    this->operators[user.client_fd] = &user;
}

void Channel::removeUser(Client& user) {
    for (it = users.begin(); it != users.end(); it++) {
        if (it->second->client_fd == user.client_fd) {
            this->users.erase(it);
            break;
        }
    }
}

void Channel::removeOperator(Client& user) {
     for (it = operators.begin(); it != operators.end(); it++) {
        if (it->second->client_fd == user.client_fd) {
            this->operators.erase(it);
            break;
        }
    }
}

void Channel::removeInvited(Client& user) {
     for (it = inviteds.begin(); it != inviteds.end(); it++) {
        if (it->second->client_fd == user.client_fd) {
            this->inviteds.erase(it);
            break;
        }
     }
}

void Channel::removeFromAll(Client& user) {
        this->removeUser(user);
        this->removeOperator(user);
        this->removeInvited(user);
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

std::map<char, int> Channel::getModes() { return this->modes; }

// returns a string with the nicknames of all users in the channel separated by a space
// with prefix @ for operators 
std::string Channel::getListOfUsers() {
    std::string listusers = "";
    std::map<int, Client*>::iterator it = users.begin();
    for (; it != users.end(); it++) {
        if (this->isOperator(*it->second))
            listusers += "@" + it->second->nickname + " ";
        else
            listusers += it->second->nickname + " ";
    }
    return trimFunction(listusers);
}

/////   SETTERS     /////

void Channel::setName(std::string name) { this->name = name; }

void Channel::setPassword(std::string password) { this->password = password; }

void Channel::setUserLimit(int userLimit) { this->userLimit = userLimit; }

void Channel::setMode(char mode, int value) { this->modes[mode] = value; }

bool Channel::isModeSet(char mode) { return this->modes[mode]; }    

void Channel::setTopic(std::string topic) { this->topic = topic; }

/////   CHECKERS    //////

int Channel::howManyUsers() { return this->users.size(); }

bool Channel::isUser(Client& user) { return this->users.find(user.client_fd) != this->users.end(); }

bool Channel::isOperator(Client& user) { return this->operators.find(user.client_fd) != this->operators.end(); }

bool Channel::hasTopic() { return this->topic != ""; }

bool Channel::isMode(char mode) { return this->modes[mode]; }


/////   UTILS   /////

std::vector<std::string> Channel::split(std::string str, char del)
{
    std::stringstream ss(str);
    std::string substr;
    std::vector<std::string> result;
    while (ss.good()) {
        getline(ss, substr, del);
        result.push_back(substr);
    }
    return result;
}

void Channel::splitChannelsName(std::vector<std::string>& params) {
    if (params.empty()) return;
    
    if (params[0].find(",") != std::string::npos) {
        std::stringstream ss(params[0]);
        std::vector<std::string> newVector;
        newVector = Channel::split(params[0], ',');
        params = newVector;
        return;
    } 

    for (size_t i = 0; i < params.size(); i++) {
        if (params[i][0] != '#' || params[i].length() < 2) {
            params.erase(params.begin() + i);
        }
    }
}






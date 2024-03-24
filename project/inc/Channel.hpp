#ifndef CHANNEL_HPP

#include "../inc/common.hpp"
#include "../inc/Reply.hpp"

#define CHANNELISFULL (471)
#define NOTONCHANNEL (442)
#define CHANOPRIVSNEEDED (482)
#define USERONCHANNEL (443)
#define UNKNOWNMODE (472)
#define INVITEONLYCHAN (473)

class User {
    public:
        int client_fd;
        std::string username;
        User() {}
        User(int fd, std::string name){
            this->client_fd = fd;
            this->username = name;
        }
        ~User() {}
};

class Channel {
    private:
        std::string chanName;
        std::map<int, User> chanUsers;
        std::map<int, User> chanOperators;
        std::map<std::string, bool> chanModes;
        std::string topic;
        int maxUsers;
        void initModes();
    public:
        Channel(const std::string& name, const User& user);
        ~Channel();
        int addUser(const User& user);
        int removeUser(const User& user);
        int setOperator(const User& user);
        int removeOperator(const User& user);
        int setMode(const User& user, std::string mode, bool value, int maxUsers);
        int setTopic(const User& user, std::string topic);
        void printUsers();

        //MESSAGES
        void sendToUser(const User& user, const std::string& message);
        void sendToChannel(const std::string& message);

        //GETTERS
        std::string getName();
        std::map<int, User> getUsers();
        std::map<int, User> getOperators();
        std::map<std::string, bool> getModes();
        std::string getTopic();
        int getNumberOfUsers();

        //CHECKERS
        bool isOperator(const User& user);
        bool isUser(const User& user);
        bool isMode(std::string mode);
        bool isModeSet(std::string mode);
        bool hasTopic();
};


Channel::Channel(const std::string& name, const User& user): chanName(name) {
    this->addUser(user);
    this->setOperator(user);
    this->initModes(); 
}

Channel::~Channel() {    }

int Channel::addUser(const User& user) {
    if (this->isUser(user)) {
        std::cout << "AddUser Error: " << user.username << " already in channel: " << this->chanName << std::endl;
        return USERONCHANNEL;
    }
    else if ((this->isModeSet("+l") && (this->getNumberOfUsers() >= this->maxUsers))) {
        std::cout << "AddUser Error: " << this->chanName << " is full" << std::endl;
        return CHANNELISFULL;
    }

    this->chanUsers.insert(std::make_pair(user.client_fd, user));
    if (this->hasTopic())
        std::cout << "Topic: " << this->topic << std::endl;
    std::cout << "AddUser: " << user.username << " added to channel: " << this->chanName << std::endl;
    return 0;
}

int Channel::removeUser(const User& user) {
    if (!this->isUser(user)) {
        std::cout << "removeUser Error: " << user.username << " not found in channel: " << this->chanName << std::endl;
        return NOTONCHANNEL;
    }
    this->chanUsers.erase(user.client_fd);
    std::cout << "removeUser: " << user.username << " removed from channel: " << this->chanName << std::endl;
    return 0;
}

int Channel::setOperator(const User& user) {
    if (!this->isUser(user)) {
        std::cout << "setOperator Error: " << user.username << " not found in channel: " << this->chanName << std::endl;
        return NOTONCHANNEL;
    } else if (this->isOperator(user)) {
        std::cout << "setOperator Error: " << user.username << " is already operator in channel: " << this->chanName << std::endl;
        return 1;
    }

    this->chanOperators.insert(std::make_pair(user.client_fd, user));
    std::cout << "setOperator: " << user.username << " is now operator in channel: " << this->chanName << std::endl;
    return 0;
}   


// to do: ....
// condidering get an operator as parameter to remove the user
int Channel::removeOperator(const User& user) {
    if (!this->isUser(user)) {
        std::cout << "removeOperator Error: " << user.username << " not found in channel: " << this->chanName << std::endl;
        return NOTONCHANNEL;
    } else if (!this->isOperator(user)) {
        std::cout << "removeOperator Error: " << user.username << " is not operator in channel: " << this->chanName << std::endl;
        return 1;
    }
    this->chanOperators.erase(this->chanOperators.find(user.client_fd));
    std::cout << "removeOperator: " << user.username << " removed from operators in channel: " << this->chanName << std::endl;
    return 0;


}

void Channel::initModes() {
    this->chanModes.insert(std::make_pair("+l", false));
}

// to do: ....
// Considering set inviite mode 
int Channel::setMode(const User& user, std::string mode, bool value, int maxUsers = 30) {
        if (!this->isUser(user)) {
            std::cout << "setMode Error: " << this->chanName << std::endl;
            return NOTONCHANNEL;
        } else if (!this->isOperator(user)) {
            std::cout << "setMode Error: " << user.username << " is not operator in channel: " << this->chanName << std::endl;
            return CHANOPRIVSNEEDED;
        } else if (!this->isMode(mode)) {
            std::cout << "setMode Error: " << mode << " not found in channel: " << this->chanName << std::endl;
            return UNKNOWNMODE;
        } else if (maxUsers < 1) {
            std::cout << "setMode Error: invalid maxUsers" << std::endl;
            return 1;
        }
        this->chanModes.find(mode)->second = value;
        std::cout << "Mode: " << mode << " set to: " << value << " in channel: " << this->chanName << std::endl;
        if (mode == "+l") {
            this->maxUsers = maxUsers;
            std::cout << "Max users set to: " << maxUsers << " in channel: " << this->chanName << std::endl;
        }
        return 0;
}

int Channel::setTopic(const User& user, std::string topic) {
    if (!this->isUser(user)) {
        std::cout << "setTopic Error: " << this->chanName << std::endl;
        return NOTONCHANNEL;
    } else if (!this->isOperator(user)) {
        std::cout << "setTopic Error: " << user.username << " is not operator in channel: " << this->chanName << std::endl;
        return CHANOPRIVSNEEDED;
    }
    this->topic = topic;
    std::cout << "Topic set to: " << topic << " in channel: " << this->chanName << std::endl;
    return 0;
}

void Channel::printUsers() {
    std::cout << "Users in channel: " << this->chanName << std::endl;
    for (std::map<int, User>::iterator it = this->chanUsers.begin(); it != this->chanUsers.end(); it++) {
        std::cout << it->second.username << std::endl;
    }
}


// ****** MESSAGES ****** //
void Channel::sendToUser(const User& user, const std::string& message) {
    send(user.client_fd, message.c_str(), message.length(), 0);
}


void  Channel::sendToChannel(const std::string& message) {
    for (std::map<int, User>::iterator it = this->chanUsers.begin(); it != this->chanUsers.end(); it++) {
        send(it->second.client_fd, message.c_str(), message.length(), 0);
        std::cout << "USER client_fd IN CHANNEL: " << it->second.client_fd << std::endl;
    }
}



// ****** GETTERS ****** //

std::string Channel::getName() {
    return this->chanName;
}

std::map<int, User> Channel::getUsers() {
    return this->chanUsers;
}

std::map<int, User> Channel::getOperators() {
    return this->chanOperators;
}

std::map<std::string, bool> Channel::getModes() {
    return this->chanModes;
}

std::string Channel::getTopic() {
    return this->topic;
}

int Channel::getNumberOfUsers() {
    return this->chanUsers.size();
}

// ****** CHECKERS ****** //

bool Channel::isOperator(const User& user) {
    if (this->chanOperators.find(user.client_fd) != this->chanOperators.end()) {
        return true;
    }
    return false;
}

bool Channel::isUser(const User& user) {
    if (this->chanUsers.find(user.client_fd) != this->chanUsers.end()) {
        return true;
    }
    return false;
}

bool Channel::isMode(std::string mode) {
    if (this->chanModes.find(mode) != this->chanModes.end()) {
        return true;
    }
    return false;
}

bool Channel::isModeSet(std::string mode) {
    if (this->chanModes.find(mode)->second) {
        return true;
    }
    return false;
}

bool Channel::hasTopic() {
    if (this->topic.empty()) {
        return false;
    }
    return true;
}

#endif





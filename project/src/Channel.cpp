#include "../inc/common.hpp"
#include "../inc/Reply.hpp"

class User {
    public:
        int client_fd;
        std::string username;
        std::string userId;
        std::string userClient;
        User() {}
        User(int client_fd, std::string name, std::string id, std::string client){
            this->client_fd = client_fd;
            this->username = name;
            this->userId = id;
            this->userClient = client;
        }
        ~User() {}
};

class Channel {
    private:
        std::string chanName;
        std::map<int, User> chanUsers;
        std::map<int, User> chanOperators;
        std::map<std::string, bool> chanModes;
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
        void printUsers();

        //MESSAGES
        void privSend(const User& user, const std::string& message);
        void broadcast(const std::string& message);

        //GETTERS
        std::string getName();
        std::map<int, User> getUsers();
        std::map<int, User> getOperators();
        std::map<std::string, bool> getModes();
        int getNumberOfUsers();

        //CHECKERS
        bool isOperator(const User& user);
        bool isUser(const User& user);
        bool isMode(std::string mode);
        bool isModeSet(std::string mode);
};


Channel::Channel(const std::string& name, const User& user): chanName(name) {
    this->addUser(user);
    this->setOperator(user);
    this->initModes(); 
}

Channel::~Channel() {    }

// Add user function looks for the user in the map and if it finds it, it returns 1, if not, it adds the user to the map and returns 0
int Channel::addUser(const User& user) {
    if (this->isUser(user)) {
        std::cout << "User: " << user.username << " already in channel: " << this->chanName << std::endl;
        return 1;
    }
    this->chanUsers.insert(std::make_pair(user.client_fd, user));
    std::cout << "User " << user.username << " added to channel: " << this->chanName << std::endl;
    // this->broadcast(RPL_JOIN(user.userId, this->chanName));
    return 0;
}

int Channel::removeUser(const User& user) {
    if (this->isUser(user)) {
        this->chanUsers.erase(user.client_fd);
        std::cout << "User " << user.username << " removed from channel: " << this->chanName << std::endl;
        return 0;
    }
    std::cout << "User: " << user.username << " not found in channel: " << this->chanName << std::endl;
    return 1;
}

int Channel::setOperator(const User& user) {
    if (this->isOperator(user)) {
        std::cout << "User: " << user.username << " is already operator in channel: " << this->chanName << std::endl;
        return 0;
    } else {
        if (this->isUser(user)) {
            this->chanOperators.insert(std::make_pair(user.client_fd, user));
            std::cout << "User: " << user.username << " is now operator in channel: " << this->chanName << std::endl;
            return 0;
        } else {
            std::cout << "User: " << user.username << " not found in channel: " << this->chanName << std::endl;
            return 1;
        }
    }
}   

int Channel::removeOperator(const User& user) {
    if (this->isOperator(user)) {
        std::map<int, User>::iterator it = this->chanOperators.find(user.client_fd);
        this->chanOperators.erase(it);
        std::cout << "User: " << user.username << " removed from operators in channel: " << this->chanName << std::endl;
        return 0;
    }
    std::cout << "User: " << user.username << " not found in operators in channel: " << this->chanName << std::endl;
    return 1;
}

void Channel::initModes() {
    this->chanModes.insert(std::make_pair("+l", false));
}

int Channel::setMode(const User& user, std::string mode, bool value, int maxUsers = 30) {
    if (this->isMode(mode)) {
        if (!this->isOperator(user) || !this->isUser(user)) {
            std::cout << "User: " << user.username << " not operator in channel: " << this->chanName << std::endl;
            return 1;
        }
        if (mode == "+l") {
            if (maxUsers < 1) {
                std::cout << "Channel: " << this->chanName << " can't set mode invalid number" << std::endl;
                return 1;
            } 
        }
        std::map<std::string, bool>::iterator it = this->chanModes.find(mode);
        it->second = value;
        std::cout << "Mode: " << mode << " set to: " << value << " in channel: " << this->chanName << std::endl;
        return 0;
    }
    std::cout << "Mode: " << mode << " not found in channel: " << this->chanName << std::endl;
    return 1;
}

void Channel::printUsers() {
    std::cout << "Users in channel: " << this->chanName << std::endl;
    for (std::map<int, User>::iterator it = this->chanUsers.begin(); it != this->chanUsers.end(); it++) {
        std::cout << it->first << std::endl;
    }
}


// ****** MESSAGES ****** //
void Channel::privSend(const User& user, const std::string& message) {
    send(user.client_fd, message.c_str(), message.length(), 0);
}


void  Channel::broadcast(const std::string& message) {
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

int Channel::getNumberOfUsers() {
    return this->chanUsers.size();
}

// ****** CHECKERS ****** //

bool Channel::isOperator(const User& user) {
    std::map<int, User>::iterator it = this->chanOperators.find(user.client_fd);
    if (it != this->chanOperators.end()) {
        return true;
    }
    return false;
}

bool Channel::isUser(const User& user) {
    std::map<int, User>::iterator it = this->chanUsers.find(user.client_fd);
    if (it != this->chanUsers.end()) {
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


// int main() {
//     User Jimmy(4, "Jimmy", "JIM123", "IRSSI");
//     User Jack(5, "Jack Johnson", "Jack234", "IRSSI");
//     User Ben(6, "Ben Harper", "BEN300", "IRSSI");


    
//     Channel channel1("Linux", Jimmy);

//     channel1.addUser(Jack);
//     channel1.addUser(Ben);
//     channel1.addUser(Jack);
//     channel1.removeUser(Jack);
//     channel1.removeUser(Jack);


//     channel1.setOperator(Jack);
//     channel1.setOperator(Jimmy);
//     channel1.removeOperator(Jimmy);
//     channel1.setOperator(Ben);

//     channel1.addUser(Jack);
//     channel1.setMode(Jack, "+l", true, 30);
//     channel1.setMode(Ben, "+l", true, 30);
//     channel1.setMode(Ben, "+l", true, 1);


// }



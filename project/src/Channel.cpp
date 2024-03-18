#include "../inc/common.hpp"
#include <unordered_map>

class User {
    public:
        std::string _name;
        User(std::string name): _name(name) {}
};

class Channel {
    private:
        std::string _name;
        std::unordered_map<std::string, const User&> _users;
        std::unordered_map<std::string, bool> _modes;
        std::vector<std::string> _operators;
    public:
        Channel(std::string name, const User& user);
        ~Channel();
        int addUser(const User& user);
        int removeUser(const User& user);
        void printUsers();
        // void setOperator(const User& user);
        // void removeOperator(const User& user);
        // void setMode(std::string mode, bool value);

        //getters
        std::string getName();
        std::unordered_map<std::string, const User&> getUsers();
        // std::unordered_map<std::string, bool> getModes();
        // std::vector<std::string> getOperators();

        //checkers
        // bool isOperator(const User& user);
        // bool isUser(const User& user);
        // bool isMode(std::string mode);
        // bool isModeSet(std::string mode);
        // bool isModeUnset(std::string mode);
};

Channel::Channel(std::string name, const User& user): _name(name) {
    this->addUser(user);
}

Channel::~Channel() {
    this->_users.clear();
    this->_modes.clear();
    this->_operators.clear();
}

int Channel::addUser(const User& user) {
    this->_users.insert(std::make_pair(user._name, user));
    return 0;
}

int Channel::removeUser(const User& user) {
    for (std::unordered_map<std::string, const User&>::iterator it = this->_users.begin(); it != this->_users.end(); it++) {
        if (it != this->_users.end()) {
            this->_users.erase(it);
            std::cout << "User " << user._name << " removed from channel " << this->_name << std::endl;
            return 0;
        }
    }
    std::cout << "User " << user._name << " not found in channel " << this->_name << std::endl;
    return 1;
}

void Channel::printUsers() {
    for (std::unordered_map<std::string, const User&>::iterator it = this->_users.begin(); it != this->_users.end(); it++) {
        std::cout << it->first << std::endl;
    }
}

std::string Channel::getName() {
    return this->_name;
}

std::unordered_map<std::string, const User&> Channel::getUsers() {
    return this->_users;
}

int main() {
    User user1("user1");
    User user2("user2");
    User user3("user3");
    
    Channel channel1("channel1", user1);
    channel1.addUser(user2);
    channel1.addUser(user3);

    channel1.printUsers();

}
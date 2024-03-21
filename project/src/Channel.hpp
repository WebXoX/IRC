#include "../inc/common.hpp"
#include "../inc/Reply.hpp"

class User {
    public:
        int fd;
        std::string userName;
        std::string userId;
        std::string userClient;
        User() {}
        User(int fd, std::string name, std::string id, std::string client){
            this->fd = fd;
            this->userName = name;
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
    public:
        Channel();
        Channel(const std::string& name, const User& user);
        ~Channel();
        int addUser(const User& user);
        int removeUser(const User& user);
        int setOperator(const User& user);
        int removeOperator(const User& user);
        void printUsers();
        // void setMode(std::string mode, bool value);

        //MESSAGES
        void privSend(const User& user, const std::string& message);
        void broadcast(const std::string& message);

        //GETTERS
        std::string getName();
        std::map<int, User> getUsers();
        std::map<int, User> getOperators();
        // std::map<std::string, bool> getModes();

        //CHECKERS
        bool isOperator(const User& user);
        bool isUser(const User& user);
        // bool isMode(std::string mode);
        // bool isModeSet(std::string mode);
        // bool isModeUnset(std::string mode);
};

Channel::Channel() {}

Channel::Channel(const std::string& name, const User& user): chanName(name) {
    this->addUser(user);
    this->setOperator(user);
}

Channel::~Channel() {    }

// Add user function looks for the user in the map and if it finds it, it returns 1, if not, it adds the user to the map and returns 0
int Channel::addUser(const User& user) {
    if (this->isUser(user)) {
        std::cout << "User: " << user.userName << " already in channel: " << this->chanName << std::endl;
        return 1;
    }
    this->chanUsers.insert(std::make_pair(user.fd, user));
    std::cout << "User " << user.userName << " added to channel: " << this->chanName << std::endl;
    // this->broadcast(RPL_JOIN(user.userId, this->chanName));
    return 0;
}

int Channel::removeUser(const User& user) {
    if (this->isUser(user)) {
        this->chanUsers.erase(user.fd);
        std::cout << "User " << user.userName << " removed from channel: " << this->chanName << std::endl;
        return 0;
    }
    std::cout << "User: " << user.userName << " not found in channel: " << this->chanName << std::endl;
    return 1;
}

int Channel::setOperator(const User& user) {
    if (this->isOperator(user)) {
        std::cout << "User: " << user.userName << " is already operator in channel: " << this->chanName << std::endl;
        return 0;
    } else {
        if (this->isUser(user)) {
            this->chanOperators.insert(std::make_pair(user.fd, user));
            std::cout << "User: " << user.userName << " is now operator in channel: " << this->chanName << std::endl;
            return 0;
        } else {
            std::cout << "User: " << user.userName << " not found in channel: " << this->chanName << std::endl;
            return 1;
        }
    }
}   

int Channel::removeOperator(const User& user) {
    if (this->isOperator(user)) {
        std::map<int, User>::iterator it = this->chanOperators.find(user.fd);
        this->chanOperators.erase(it);
        std::cout << "User: " << user.userName << " removed from operators in channel: " << this->chanName << std::endl;
        return 0;
    }
    std::cout << "User: " << user.userName << " not found in operators in channel: " << this->chanName << std::endl;
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
    send(user.fd, message.c_str(), message.length(), 0);
}


void  Channel::broadcast(const std::string& message) {
    for (std::map<int, User>::iterator it = this->chanUsers.begin(); it != this->chanUsers.end(); it++) {
        send(it->second.fd, message.c_str(), message.length(), 0);
        std::cout << "USER FD IN CHANNEL: " << it->second.fd << std::endl;
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

// ****** CHECKERS ****** //

bool Channel::isOperator(const User& user) {
    std::map<int, User>::iterator it = this->chanOperators.find(user.fd);
    if (it != this->chanOperators.end()) {
        return true;
    }
    return false;
}

bool Channel::isUser(const User& user) {
    std::map<int, User>::iterator it = this->chanUsers.find(user.fd);
    if (it != this->chanUsers.end()) {
        return true;
    }
    return false;
}

// int main() {
//     User user1(4, "Jimmy", "JIM123", "IRSSI");
//     User user2(5, "Jack Johnson", "Jack234", "IRSSI");
//     User user3(6, "Ben Harper", "BEN300", "IRSSI");
    
//     Channel channel1("Music", user1);
//     channel1.addUser(user2);
//     channel1.addUser(user2);
//     channel1.removeUser(user2);
//     channel1.setOperator(user1);
//     channel1.removeOperator(user1);

//     channel1.printUsers();

// }





// #include <unordered_map>
// #include <string>
// #include <vector>

// class User; // Forward declaration assuming a User class exists

// class Channel {
// public:
//   Channel(const std::string& name);

//   const std::string& getchanName() const { return name_; }

//   // addUser/Part logic
//   void addUser(User* user);
//   void part(User* user);

//   // Message broadcast
//   void send_message(const std::string& message, User* sender = nullptr);

//   // Topic management
//   void set_topic(User* user, const std::string& topic);
//   const std::string& get_topic() const { return topic_; }

//   // Mode management
//   bool set_mode(const std::string& mode, bool value);
//   bool has_mode(const std::string& mode) const { return modes_.count(mode) > 0; }

//   // User list and permissions
//   std::vector<std::string> get_user_list() const;
//   bool has_permission(User* user, const std::string& permission) const;

// private:
//   // Channel data
//   std::string name_;
//   std::unordered_map<std::string, User*> users_;
//   std::string topic_;
//   std::unordered_map<std::string, bool> modes_; // Mode name -> bool value
//   std::vector<std::string> operators_; // List of operator nicknames

//   // Helper methods
//   bool is_operator(const std::string& nickname) const;
// };

// Channel::Channel(const std::string& name) : name_(name) {}

// // Implementations for addUser, part, send_message, etc. (see function comments)

// // Example implementation for send_message
// void Channel::send_message(const std::string& message, User* sender) {
//   for (auto& user : users_) {
//     if (user.usernd != sender) {
//       // Call user object's message sending function here (replace with your logic)
//       user.second->send_message(message);
//     }
//   }
// }

// // Implementations for other methods following similar logic

// bool Channel::is_operator(const std::string& nickname) const {
//   return std::find(operators_.begin(), operators_.end(), nickname) != operators_.end();
// }

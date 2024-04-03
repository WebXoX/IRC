#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "../inc/common.hpp"
#include "../inc/Reply.hpp"
#include "../inc/Client.hpp"


class Channel {
    private:
        std::string name;
        std::map<int, Client*> users;
    public:
        Channel(const std::string& name, Client& user);
        ~Channel();

        int addUserInChannel(Client& user);
        bool isUserInChannel(Client& user);

        // ****** GETTERS ****** //
        std::string getName() ;
        
};

#endif
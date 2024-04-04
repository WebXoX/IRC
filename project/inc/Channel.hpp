#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "common.hpp"
#include "Reply.hpp"
#include "Client.hpp"
#include "parsing.hpp"


class Channel {
    public:
        std::string name;
        std::map<int, Client> users;
        std::map<int, Client> operators;

        Channel();
        Channel(std::string& name, Client& user);
        Channel(const Channel& copy);
        Channel& operator=(const Channel& copy);
        ~Channel();

        std::string addUserInChannel(Client& user);
        std::string setChannelOperator(Client& user);

        // ****** CHECKERS ****** //
        bool isUserInChannel(Client& user);
        bool isUserOperator(Client& user);
};

#endif
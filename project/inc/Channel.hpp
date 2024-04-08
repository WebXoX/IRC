#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "common.hpp"
#include "Reply.hpp"
#include "Client.hpp"
#include "parsing.hpp"


class Channel {
    public:
        std::string name;
        std::map<int, Client*> users;
        std::map<int, Client*> operators;
        std::vector<std::string> dataBase;
        std::map<int, int> userMessages;

        Channel();
        Channel(std::string& name, Client& user);
        Channel(const Channel& copy);
        Channel& operator=(const Channel& copy);
        ~Channel();

        std::string addUserInChannel(Client& user);
        std::string setChannelOperator(Client& user);
        void broadcastMessage(std::string message);
        void announceNewUser(Client& user);
        int howManyUsersInChannel();
        std::string getUserMessages(Client& user);
        std::string addMessageDatabase(std::string message);

        // ****** GENERATE MACROS ****** //
        std::string welcomeMessage(Client& user);
        std::string listOfUsers();

        // ****** CHECKERS ****** //
        bool isUserInChannel(Client& user);
        bool isUserOperator(Client& user);
};

#endif
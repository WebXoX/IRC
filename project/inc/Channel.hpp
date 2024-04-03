#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "../inc/common.hpp"
#include "../inc/Reply.hpp"
#include "../inc/Client.hpp"

#define CHANNELISFULL (471)
#define NOTONCHANNEL (442)
#define CHANOPRIVSNEEDED (482)
#define USERONCHANNEL (443)
#define UNKNOWNMODE (472)
#define INVITEONLYCHAN (473)


class Channel {
    private:
        std::string chanName;
        std::map<int, Client> chanUsers;
        std::map<int, Client> chanOperators;
        std::map<std::string, bool> chanModes;
        std::string topic;
        int maxUsers;
        void initModes();
    public:
        Channel(const std::string& name, const Client& user);
        ~Channel();
        int addUser(const Client& user);
        int removeUser(const Client& user);
        int setOperator(const Client& user);
        int removeOperator(const Client& user);
        int setMode(const Client& user, std::string mode, bool value, int maxUsers);
        int setTopic(const Client& user, std::string topic);
        void printUsers();

        //MESSAGES
        void sendToUser(const Client& user, const std::string& message);
        void sendToChannel(const std::string& message);

        //GETTERS
        std::string getName();
        std::map<int, Client> getUsers();
        std::map<int, Client> getOperators();
        std::map<std::string, bool> getModes();
        std::string getTopic();
        int getNumberOfUsers();

        //CHECKERS
        bool isOperator(const Client& user);
        bool isUser(const Client& user);
        bool isMode(std::string mode);
        bool isModeSet(std::string mode);
        bool hasTopic();
};

#endif
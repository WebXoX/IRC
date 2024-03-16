#ifndef __PARSING_HPP__
#define __PARSING_HPP__

#include "common.hpp"

struct ircMessage {
    int messageLength;
    std::string message;
    std::string command;
    std::vector<std::string> params;
    std::string trailing;
    int errorFlag;
    std::string errorMessage;
};

std::string trimFunction(std::string str);
ircMessage parseMessage(std::string message);
void printCommand(ircMessage message);

#endif
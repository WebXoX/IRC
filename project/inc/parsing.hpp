#pragma once

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

ircMessage parseMessage(std::string message);
void printCommand(ircMessage message);

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "common.hpp"

class Request
{
    private:
    public:
        std::string input;
        std::string command;
        std::vector<std::string> args;
        std::string trailing;
        bool isValidInput;
        Request();
        ~Request();
};

Request::Request() { this->isValidInput = false; }
Request::~Request() {}

#endif
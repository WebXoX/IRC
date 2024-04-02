#ifndef PARSER_HPP
#define PARSER_HPP

#include "common.hpp"
#include "Request.hpp"

class Parser {
    private:
        Request request;
    public:
        Parser();
        ~Parser();
        int parsing(std::string input);
        Request getRequest();
        
};

Parser::Parser() {}

Parser::~Parser() {}

// this is just a basic parsing to split the string still a lot of work to be done
int Parser::parsing(std::string input) {
    std::istringstream iss(input);
    std::string token;

    if (input.empty() || input[0] == ' ')
        return 1;
    bool cmd = true;
    while (std::getline(iss, token, ' ')) {
        if (cmd) {
            cmd = false;
            request.command = token;
            continue;
        }
        this->request.args.push_back(token);
    }
    this->request.isValidInput = true;
    return 0;
}

Request Parser::getRequest() { return this->request; }



#endif
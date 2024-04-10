#include "../inc/parsing.hpp"

std::string trimFunction(std::string str)
{
    size_t first = str.find_first_not_of(" \n\r\t");
    size_t last = str.find_last_not_of(" \n\r\t");

    if (first == std::string::npos || last == std::string::npos) {
        return "";
    }

    return str.substr(first, (last - first + 1));
}

bool isValidParam(std::string param)
{
    std::string invalidParam = "\n\r\t:";
    size_t found = param.find_first_of(invalidParam);
    return found == std::string::npos;
}

ircMessage parseMessage(std::string message)
{
    if (message.empty()) {
        ircMessage emptyMessage;
        return emptyMessage;
    }

    ircMessage parsedMessage;
    parsedMessage.message = message;
    parsedMessage.messageLength = message.length();
    parsedMessage.errorFlag = 0;
    parsedMessage.trailing = "";
    std::string trimmedMessage = trimFunction(message);
    std::string token;
    std::istringstream iss(trimmedMessage);
    bool tagFlag = false;
    bool tagCmd = true;


    if (trimmedMessage[0] == '@') 
        tagFlag = true;

    while (iss >> token) {
        if (tagFlag) {
            if (token[0] == ':') {
                parsedMessage.command = token.substr(1);
                tagFlag = false;
                tagCmd = false;
                continue;
            }
            continue;
        }

        if (tagCmd) {
            parsedMessage.command = token;
            tagCmd = false;
            continue;
        }

        if (token[0] == ':') {
            parsedMessage.trailing = trimFunction(message.substr(message.find(token) + 1));
            break;
        } else {
            if (isValidParam(token))
                parsedMessage.params.push_back(token);
            else {
                parsedMessage.errorFlag = 1;
                parsedMessage.errorMessage = "Invalid parameter: " + token;
                break;
            }
        }

    }

    return parsedMessage;
}

void printCommand(ircMessage message)
{
    if (message.errorFlag == 1) {
        std::cout << message.errorMessage << std::endl;
        return;
    }

    std::cout << "\nMessage: " << message.message << std::endl;
    std::cout << "Message Length: " << message.messageLength << std::endl;
    std::cout << "Command: " << message.command << std::endl;
    for (size_t i = 0; i < message.params.size(); i++) {
        std::cout << "Param " << i << ": " << message.params[i] << std::endl;
    }
    std::cout << "Trailing: " << message.trailing << std::endl;
}


// int main(int argc , char *argv[]){

//     while (true)
//     {
//         std::string userInput;

//         std::cout << "Enter a message: ";
//         std::getline(std::cin, userInput);
//         printCommand(parseMessage(userInput));
//     }
    
//     return 0;
// }
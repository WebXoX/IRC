#include "../inc/common.hpp"
#include "../inc/Channel.hpp"

class User;
class Channel;

int main() {
    
    std::string fullMessage;
    User tehuan(1, "tehuan");

    while (true) {
        std::string messageChunk;

        std::cout << "type your message: ";
        std::getline(std::cin, messageChunk);
        if (messageChunk == "exit") {
            break;
        }

        fullMessage += messageChunk;
        if (fullMessage.size() >= 2 && (fullMessage[fullMessage.size() - 2] == '\r' && fullMessage.back() == '\n') || sizeof(fullMessage) > 512)
            break;

    }
    

    std::cout << "FULL MESSAGE: " << fullMessage << std::endl;
}
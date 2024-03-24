#include "../inc/common.hpp"

class Input {
    private:
        std::string input;
        std::string command;
        std::vector<std::string> params;
        std::vector<std::string> words;
        std::vector<std::string> commandsAvailable;
        std::string trail;
        bool commandFlag;
        void initInput();
    public:
        Input(std::string str);
        ~Input();
        std::string getCommand();
        std::vector<std::string> getParams();
        std::string getTrail();

        int parseInput(std::string input);

        bool isCommand(std::string cmd) {
            return std::find(this->commandsAvailable.begin(), this->commandsAvailable.end(), cmd) != this->commandsAvailable.end();
        }
};

Input::Input(std::string str) : input(str) {
    this->initInput();
    this->parseInput(this->input);
}

Input::~Input() {}

void Input::initInput() {
    this->commandFlag = false;
    this->commandsAvailable.push_back("NICK");
    this->commandsAvailable.push_back("JOIN");
}

int Input::parseInput(std::string input)
{
    if (input.empty()) {
        std::cerr << "Empty input" << std::endl;
        return 1;
    }

    std::string token;
    std::istringstream iss(input);

    while (iss >> token)
        this->words.push_back(token);
    
    
}
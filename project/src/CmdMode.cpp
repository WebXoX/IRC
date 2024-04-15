#include "../inc/Server.hpp"

// 21:24 Syntax:
// 21:24 
// 21:24 MODE <your nick>|<chanName> [<mode> [<mode parameters>]]
// 21:24 
// 21:24 Parameters:
// 21:24 
// 21:24     The target nickname or chanName and the modes with their parameters to set or
// 21:24     remove.
// 21:24 
// 21:24     If the target nickname or chanName is omitted, the active nickname or chanName
// 21:24     will be used.
// 21:24 
// 21:24 Description:
// 21:24 
// 21:24     Modifies the user or chanName modes for which you are privileged to modify.
// 21:24 
// 21:24     You can specify multiple modes in one command and prepend them by using the
// 21:24     '+' sign to set or '-' sign to unset; modes that require a parameter will be
// 21:24     retrieved from the argument list.
// 21:24 
// 21:24     Some common chanName modes are:

// 21:24         i:               When enabled, users need to be invited into the chanName
// 21:24                          or have a matching entry in the invite list.
// 21:24         k <key>:         Adds or removes a chanName key, aka a password; users
// 21:24                          will not be able to join the chanName without providing
// 21:24                          the key.
// 21:24         l <count>:       Adds, removes or updates the maximum amount of users
// 21:24         t:               When enabled, users who are not opped or voices cannot
// 21:24                          modify the chanName topic.
// 21:24         o <nickname>:    Adds or removes the operator status from a nickname..
// 21:24 
// 21:24     /MODE #irssi +o mike
// 21:24     /MODE #irssi -o+iI mike mike!*@*.irssi.org
// 21:24     /MODE +b mike!*@*.irssi.org
// 21:24     /MODE -o mike
// 21:24     /MODE +impsnkl secret_hideout 100
// 21:24     /MODE mike +iw

std::map<char, std::pair<int, std::string> > getModes(Channel channel, std::string modestring, std::vector<std::string> modeParams) {
    
    std::map<char, std::pair<int, std::string> > modes; // map to store the values the key is the mode 'x' the value is a pair<int,str> the int is the value to be set on/off the string is the mode parameter
    int flag; // variable to store the value on/off
    int paramIndex = 0;
    

    for (size_t i = 0; i < modestring.length(); i++) {
            char c = modestring[i];
            if (c == '+')
                flag = 1;
            else if (c == '-')
                flag = 0;
            else if (c == 'k' || c == 'l') { // these two are special modes (key and limit) that require parameters
                if (flag) { // if the mode is to set on a parameter is required
                    if (!modeParams.empty()) {
                        if (c == 'l') {
                            int newLimit = std::atoi(modeParams[paramIndex].c_str());
                            if (newLimit == 0 && modeParams[paramIndex] != "0") {
                                paramIndex++;
                                continue;
                            }
                        }
                        modes[c] = std::make_pair(flag, modeParams[paramIndex++]); // paramIndex is used to track wich parameter was already taken, we get the value and increment it for the next
                    }
                } 
                else {
                    if (channel.isModeSet(c) != flag)
                        modes[c] = std::make_pair(flag, ""); // set the parameter value as empty string
                }
            } 
            else if (c == 'o') { // the mode operator requires parameter either for on and off
                if (!modeParams.empty()) { // if there is parameter on the parameters vector
                    if (channel.isUser(modeParams[paramIndex]))
                        modes[c] = std::make_pair(flag, modeParams[paramIndex++]); // store the mode into the modes map
                    else {
                        paramIndex++;
                    }
                }
            }
            else if (c == 'i' || c == 't') {
                if (channel.isModeSet(c) != flag)
                    modes[c] = std::make_pair(flag, ""); // store them and the parameter is an empty string
            }
        
        }

        return modes;
}

std::string getModestringSeted(std::map<char, std::pair<int, std::string> > modes) {
    std::string plusModes = "";
    std::string minusModes = "";
    std::string modesParams = "";
    std::map<char, std::pair<int, std::string> >::iterator it;
    for (it = modes.begin(); it != modes.end(); ++it) {
        char c = it->first;
        std::string str(1, c);
        if (it->second.first == 1) {
            plusModes += str;
            modesParams += it->second.second+" ";
        } 
        else
            minusModes += str;
    }
    std::string finalstring = "";
    minusModes.empty() ? "" : finalstring += "-"+minusModes;
    plusModes.empty() ? "" : finalstring += "+"+plusModes + " "+trimFunction(modesParams);
    
    return finalstring;
}

bool validateNumberModeParameters(std::string modestring, std::vector<std::string> modeParams) {
    size_t counter = 0;
    for (size_t i = 0; i < modestring.length(); i++) {
        if (modestring[i] == 'k' || modestring[i] == 'l' || modestring[i] == 'o') {
            counter++;
        }
    }
    return counter > 0 ? counter == modeParams.size() : true;
}

void Server::modeCommand(ircMessage& msg, Client& user) 
{
    (void )user;
    // ************************************************************* //
    //  this block extracts the variables needed to set the modes
    //  <channel> <modestring> [<parameters>]
    // ************************************************************* //
    std::string channel;
    std::string modestring;
    std::vector<std::string> modeParams;

    for (size_t i = 0; i < msg.params.size(); i++) {
        if (i == 0)
            channel = msg.params[i];
        else if (i == 1)
            modestring = msg.params[i];
        else            
            modeParams.push_back(msg.params[i]);
    }

    if (!msg.trailing.empty()) {
        std::vector<std::string> tokens = split(msg.trailing, ' ');
        for (size_t i = 0; i < tokens.size(); i++) {
            if (i == 0) { // if is the first word from the trailing
                modeParams.push_back(tokens[i].substr(1));  // remove the colom and store into the mode params
                continue;
            }
            modeParams.push_back(tokens[i]);
        }
    }


    // ************************************************************* //
    // ************************************************************* //
    // ************************************************************* //
    //  this block is the first error handling layer
    //  checks if the number of parameter is valid
    //  if the channel exists
    //  if the user has operator privileges
    // ************************************************************* //
    std::string reply;
    this->chan_it = this->getChannelIt(channel);
    if (msg.params.size() < 1) {
        reply = ERR_NEEDMOREPARAMS(user.nickname, "MODE");
    }
    else if (msg.params[0][0] != '#')
    {
        std::string target = msg.params[0];
        if(this->isUserNick(target))
        {
            if(msg.params.size() > 1)
                reply = ERR_UMODEUNKNOWNFLAG(user.nickname);
            else
                reply =  RPL_CHANNELMODEIS(user.nickname,"", "");
        }
        else
            reply = ERR_USERSDONTMATCH(user.nickname);
    }
    else if (chan_it == this->channels.end()) {
        reply = ERR_NOSUCHCHANNEL(user.nickname, channel);
    }
    else if (modestring.find_first_not_of("+-kloit") != std::string::npos) {
        std::size_t charPosition = modestring.find_first_not_of("+-kloit");
        char c = modestring[charPosition];
        std::string charString(1, c);
        reply = ERR_UNKNOWNMODE(user.nickname, charString);
    }
    else if (msg.params.size() == 1 && this->channels[channel].isUser(user)) {
        reply = RPL_CHANNELMODEIS(user.nickname, channel, this->channels[channel].getModes());
    }
    else if (msg.params.size() > 1 && !chan_it->second.isOperator(user)) {
        reply = ERR_CHANOPRIVSNEEDED(user.nickname, channel);
    }
    else {

        std::map<char, std::pair<int, std::string> > modes;
        this->chan_it = this->getChannelIt(channel);
        if (this->chan_it != this->channels.end())
            modes = getModes(chan_it->second, modestring, modeParams);

        std::map<char, std::pair<int, std::string> >::iterator it;
        for (it = modes.begin(); it != modes.end(); ++it) {

            int flag = it->second.first;
            std::string parameter = it->second.second;
            char mode = it->first;

            std::cout << "mode: " << mode << " / " << "flag: " << flag << " / " << "parameter: " << (parameter.empty() ? "no patameter" : parameter) << std::endl;

            if (mode == 'l' && flag) {
                    this->channels[channel].setUserLimit(std::atoi(parameter.c_str()));
                    std::cout << "user limit set: " << this->channels[channel].getUserLimit() << std::endl;
            } 
            else if (mode == 'k' && flag) {
                this->channels[channel].setPassword(parameter);
                std::cout << "password set: " << this->channels[channel].getPassword() << std::endl;
            } 
            else if (mode == 'o') {
                Client* client = this->getUser(parameter);
                    if (client != NULL) {
                        if (client &&this->channels[channel].isUser(*client)) {
                            if (flag == 1) {
                                if (!this->channels[channel].isOperator(*client)) {
                                    this->channels[channel].addOperator(*client);
                                }
                                else
                                    std::cout << client->nickname << " is already an operator of " << channel << std::endl;
                            }
                            else if (flag == 0) {
                                if (this->channels[channel].isOperator(*client)) {
                                    this->channels[channel].removeOperator(*client);
                                }
                                else
                                    std::cout << client->nickname << " is not an operator of " << channel << std::endl;
                            }
                        } 
                        else
                            std::cout << client->nickname << " is not an user of " << channel << std::endl;
                    }
                    else
                        std::cout << parameter << " is not an user in the server " << std::endl;
                continue;
            }
            
            

            this->channels[channel].setMode(mode, it->second.first);      // /mode +ti
        }

        if (!modes.empty()) {
            std::string reply = MODE_CHANNELCHANGEMODE(user_id(user.nickname, user.username), channel, getModestringSeted(modes));
            this->channels[channel].broadcast(reply);
            return;
        }
    }

    this->definedmessage(user.client_fd, reply.c_str());



}
   
#include "../inc/Server.hpp"

void Server::privmsgCommand(ircMessage msg, Client& user) {
    if (msg.params.size() == 1) {
        
        std::string target = msg.params[0];
        if(msg.trailing.empty()== false)
        {
            std::string message = msg.trailing;
            if(target[0] =='#')
            {
                if(this->hasChannelInServer(target))
                {
                    if(this->channels[target].isUser(user))
                        this->channels[target].broadcast_others(user,RPL_PRIVMSG(user.nickname, user.username, target, message));
                }
                else
                    user.toSend.push_back(ERR_CANNOTSENDTOCHAN(user.nickname, target));    
            }
            else
            {
                if(this->isUserNick(target))
                    this->getUser(target)->toSend.push_back(RPL_PRIVMSG(user.nickname, user.username, target, message));   
                else
                    user.toSend.push_back(ERR_NOSUCHNICK(user.nickname, target));
            }
        }
        else
            user.toSend.push_back(ERR_NORECIPIENT(user.nickname));
    }
    else
        user.toSend.push_back(ERR_NOTEXTTOSEND(user.nickname));
}

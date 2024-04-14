#include "../inc/Server.hpp"
//  ∗ MODE- Change the channel’s mode:
//  · i: Set/remove Invite-only channel
//  · t: Set/remove the restrictions of the TOPIC command to channel
//  operators
//  · k: Set/remove the channel key (password)
//  · o: Give/take channel operator privilege
//  · l: Set/remove the user limit to channel
// int Server::modetype(char mode)
// {
//     if(mode == 'i')
//         return 1;
//     else if(mode == 't')
//         return 2;
//     else if(mode == 'k')
//         return 3;
//     else if(mode == 'o')
//         return 4;
//     else if(mode == 'l')
//         return 5;
//     return 0;
// }

int Server::modechange(std::map<std::string,std::string> mode, Channel& channel, Client& user)
{
    std::map<std::string,std::string>::iterator it;
    std::string param;
    for(it=mode.begin();it != mode.end();++it )
    {
        char sign = it->first[0];
        if(it->first[1] == 'i')
        {
            if(sign == '+')
                channel.setMode('i',1);
            else
                channel.setMode('i',0);
        }
        else if(it->first[1] == 't')
        {
            if(sign == '+')
                channel.setMode('t',1);
            else
                channel.setMode('t',0);
        }
        else if(it->first[1] == 'k')
        {
            if(sign == '+')
            {
                channel.setPassword(it->second);
                channel.setMode('k',1);
                // this->definedmessage(channel.getUsers(), RPL_CHANNELMODEIS(channel.getName(),channel.getName()));
            }
            else
            {
                channel.setPassword(NULL);
                channel.setMode('k',0);
            }
            param += it->second + "";
        }
        else if(it->first[1] == 'o')
        {
            if(this->isUserNick(it->second) )
            {
                if(sign == '+')
                {
                    channel.addOperator(*getClient(it->second));
                }
                else
                {
                    channel.removeOperator(*getClient(it->second));
                }
                param += it->second + "";
            }
        }
        else if(it->first[1] == 'l')
        {
            if(sign == '+')
            {
                channel.setUserLimit(std::strtol(it->second.c_str(),NULL,10));
                channel.setMode('l',1);
            }
            else
            {
                if(it->second == "\0")
                {
                    channel.setUserLimit(0);
                    channel.setMode('l',0);
                }
                
            }
                param += it->second + "";
        }
        else
            return 0;
        // channel.broadcast(RPL_ADDVOICE(user.nickname,user.username,channel.getName(),str,param));
    return 1;
}
return 0 ;
}
bool isModeSet(std::string mode,int flag)
{
  if(flag == 1)
  {
    if(mode.find_first_not_of("+-itokl") != std::string::npos)
        return false;
  }
  return true;
}
int findParam(std::vector<std::string> params,std::string str, size_t skip)
{   

    if (str.find_first_of("okl")!=std::string::npos)
    {
        if(params.size() > skip)
            return skip;
    }
    return -1;
}
std::map<std::string,std::string> Server::modeParser(ircMessage msg)
{
    std::map<std::string,std::string> mode;
    std::string sign;
    std::string param;
    int paramId = 2;
    int index;
    for (size_t j = 0; j < msg.params[1].size(); j++)
    {
        param = msg.params[1][j];
        if(msg.params[1][j] == '+' )
            sign = '+';
        else if(msg.params[1][j] == '-')
            sign = '-';
        else if (param.find_first_of("itokl") != std::string::npos)
        {
            index  = findParam(msg.params,param,paramId);
            param = &msg.params[1][j];
            if(index != -1)
            {
                mode[sign+msg.params[1][j]] = msg.params[paramId];
                paramId++;
            }
            else
                mode[sign+msg.params[1][j]] = "\0";
        }
        else
        {
            std::map<std::string,std::string> mode1;
            return mode1;
        }
    }
    return mode;
}
void Server::modeCommand(ircMessage msg, Client& user) {
    std::string target = msg.params[0];
    if(target[0] =='#')
    {
        if(this->hasChannelInServer(target))
        {
            if(getChannel(target).isOperator(user) == false)
            {
                this->definedmessage(user.client_fd, ERR_CHANOPRIVSNEEDED(user.nickname,target));
            }
            else if(msg.params.size() > 1)
            {
                std::map<std::string,std::string> mode  = modeParser(msg);
                if(this->channels[target].isOperator(user) == true)
                    modechange(mode ,this->channels[target],user);
            }
            else
                this->definedmessage(user.client_fd, RPL_CHANNELMODEIS(user.nickname,target, target));
        }
        else
            this->definedmessage(user.client_fd,  ERR_NOSUCHCHANNEL(user.nickname, target));    
    }
    else
    {
        if(this->isUserNick(target))
        {
            if(msg.params.size() > 0)
            {
                this->definedmessage(user.client_fd, ERR_UMODEUNKNOWNFLAG(user.nickname));
            }
            else
            {
                this->definedmessage(user.client_fd,  RPL_CHANNELMODEIS(user.nickname,target, target));
            }
        }
        else
            this->definedmessage(user.client_fd,  ERR_USERSDONTMATCH(user.nickname));    
    }
       
}

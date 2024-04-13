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
// 21:24 
// 21:24         b <mask>:        Adds or removes a ban; a ban will prevent a user who
// 21:24                          matches the given mask from joining the chanName unless
// 21:24                          he or she is invited or in the exempt list.
// 21:24         e <mask>:        Adds or removes a ban exception; users who match a mask
// 21:24                          on the exempt list are able to join a chanName even if
// 21:24                          they also match an entry on the ban list.
// 21:24         i:               When enabled, users need to be invited into the chanName
// 21:24                          or have a matching entry in the invite list.
// 21:24         I <mask>:        Adds or removes an invite; users who match a mask on
// 21:24                          the invite list are able to join even if the chanName is
// 21:24                          set to invite only.
// 21:24         k <key>:         Adds or removes a chanName key, aka a password; users
// 21:24                          will not be able to join the chanName without providing
// 21:24                          the key.
// 21:24         l <count>:       Adds, removes or updates the maximum amount of users
// 21:24                          that may join the chanName.
// 21:24         m:               When enabled, users who are not opped or voiced cannot
// 21:24                          send messages to the chanName.
// 21:24         n:               When enabled, users who are not on the chanName cannot
// 21:24                          send messages to it.
// 21:24         p:               When enabled, the chanName will not be displayed in your
// 21:24                          WHOIS output.
// 21:24         s:               When enabled, the chanName will not be displayed in the
// 21:24                          LIST output.
// 21:24         t:               When enabled, users who are not opped or voices cannot
// 21:24                          modify the chanName topic.
// 21:24         o <nickname>:    Adds or removes the operator status from a nickname.
// 21:24         v <nickname>:    Adds or removes the voice status from a nickname.
// 21:24 
// 21:24     Some common user modes are:
// 21:24 
// 21:24         i:               When enabled, other users will not be able to see you
// 21:24                          on a WHO output unless they are in the chanName
// 21:24                          themselves.
// 21:24         w:               When enabled, you will receive WALLOP messages from IRC
// 21:24                          operators.
// 21:24         s:               When enabled, you will receive special notices or debug
// 21:24                          messages from the server.
// 21:24 
// 21:24     Many networks have additional user and chanName modes; we highly recommend
// 21:24     you to read the documentation of the networks you frequently connect to and
// 21:24     maximize your IRC experience.
// 21:24 
// 21:24 Examples:
// 21:24 
// 21:24     /MODE #irssi +o mike
// 21:24     /MODE #irssi -o+iI mike mike!*@*.irssi.org
// 21:24     /MODE +b mike!*@*.irssi.org
// 21:24     /MODE -o mike
// 21:24     /MODE +impsnkl secret_hideout 100
// 21:24     /MODE mike +iw


// void Server::modeCommand(ircMessage& msg, Client& user) {

//     std::string reply;
//     std::cout << "im inside the mode" << std::endl;
//     if (msg.params.size() < 1) {
//         reply = ERR_NEEDMOREPARAMS(user.username, "MODE");
//         send(user.client_fd, reply.c_str(), reply.length(), 0);
//         return;
//     }
    
//     if (msg.params.size() >= 1) {
//         if (this->hasChannelInServer(msg.params[0])) {
//             if (msg.params.size() == 1) {
//                 reply = RPL_CHANNELMODEIS(user.nickname, msg.params[0], this->channels[msg.params[0]].getModes());
//                 send(user.client_fd, reply.c_str(), reply.length(), 0);
//                 return;
//             }

//         }
//         else {
//             std::cout << "theres no channel" << std::endl;
//             reply = ERR_NOSUCHCHANNEL(user.nickname, msg.params[0]);
//             send(user.client_fd, reply.c_str(), reply.length(), 0);
//             return;
//         }

        
//     }

    


// }
       
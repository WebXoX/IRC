#include "../inc/Server.hpp"

//  Command: INVITE
//   Parameters: <nickname> <channel>

// The INVITE command is used to invite a user to a channel. 
// The parameter <nickname> is the nickname of the person to be invited to the target channel <channel>.

// The target channel SHOULD exist (at least one user is on it). 
// Otherwise, the server SHOULD reject the command with the ERR_NOSUCHCHANNEL numeric.

// Only members of the channel are allowed to invite other users. 
// Otherwise, the server MUST reject the command with the ERR_NOTONCHANNEL numeric.

// Servers MAY reject the command with the ERR_CHANOPRIVSNEEDED numeric. 
// In particular, they SHOULD reject it when the channel has invite-only mode set, 
// and the user is not a channel operator.

// If the user is already on the target channel, the server MUST reject the command 
// with the ERR_USERONCHANNEL numeric.

// When the invite is successful, the server MUST send a RPL_INVITING numeric to 
// the command issuer, and an INVITE message, with the issuer as <source>, to the 
// target user. Other channel members SHOULD NOT be notified.

// :calcium.libera.chat 341 tehuan bigboss #tehuan_worlds
// :tehuan!~somebody@5.195.225.158 INVITE bigboss :#tehuan_worlds

// Numeric Replies:

// RPL_INVITING (341)
// ERR_NEEDMOREPARAMS (461)
// ERR_NOSUCHCHANNEL (403)
// ERR_NOTONCHANNEL (442)
// ERR_CHANOPRIVSNEEDED (482)
// ERR_USERONCHANNEL (443)


// void Server::inviteCommand(ircMessage msg, Client& user) {
//     std::string reply = "";
//     std::string invited = msg.params[0];
//     std::string chanName = msg.params[1];
//     Channel& channel = this->channels[chanName];

//     if (msg.params.size() != 2) {
//         reply = ERR_NEEDMOREPARAMS(user.nickname, "INVITE");
//     } else if (!this->hasChannelInServer(chanName) || channel.howManyUsers() == 0) {
//         reply = ERR_NOSUCHCHANNEL(user.nickname, chanName); 
//     } else if (!channel.isUser(user)) {
//         reply = ERR_NOTONCHANNEL(user.nickname, chanName);
//     } else if (channel.isModeSet('i') && !channel.isOperator(user)) {
//         reply = ERR_CHANOPRIVSNEEDED(user.nickname, chanName);
//     } else {
//         for (size_t i = 0; i < this->client.size(); i++) {
//             if (this->client[i]->nickname == invited)

//         }
//                 break;
//         reply = RPL_INVITE();
//         send(user.client_fd, reply.c_str(), reply.size(), 0);
//     }
    
//     reply = RPL_INVITING(user_id(user.nickname, user.username), user.nickname, invited, chanName);
//     send(user.client_fd, reply.c_str(), reply.size(), 0);
    
// }
#include "inc/common.hpp"

std::vector<std::string> split(std::string str, char delimiter) {
    std::vector<std::string> tokens;
    std::string substr;
    std::istringstream ss(str);
    while (std::getline(ss, substr, delimiter)) {
        tokens.push_back(substr);
    }
    return tokens;
}

std::map<std::string, std::string> createChannelKeyMap(std::vector<std::string> args) {
    std::vector<std::string> channelList = split(args[0], ',');
    std::vector<std::string> keyList = split(args[1], ',');

    std::map<std::string, std::string> channelKeyMap;

    for (std::size_t i = 0; i < channelList.size(); ++i) {
        if (i < keyList.size()) {
            channelKeyMap[channelList[i]] = keyList[i];
        } else {
            channelKeyMap[channelList[i]] = "";
        }
    }

    return channelKeyMap;
}

int main(int ac, char** av) {
    std::vector<std::string> args;
    args.push_back("#channel1,#channel2,#channel3");
    // args.push_back("key1,key2");

  std::map<std::string, std::string> channels;

  channels = createChannelKeyMap(args);

  std::map<std::string, std::string>::iterator it;
  for (it = channels.begin(); it != channels.end(); it++) {
    std::cout << it->first << " => " << it->second << std::endl;
    
  }



  return 0;
}
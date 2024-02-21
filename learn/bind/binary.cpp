#include <iostream>
#include <string>
#include <vector>

// Function to convert IPv4 address string to binary representation
std::vector<unsigned char> convertIPv4ToBinary(const std::string& ipString) {
    std::vector<unsigned char> binaryIP;
    std::string byteStr;
    
    // Parse the IP string and split it into octets
    for (char c : ipString) {
        if (c == '.') {
            // Convert the byte string to an integer and append to the binary representation
            binaryIP.push_back(std::stoi(byteStr));
            byteStr.clear();
        } else {
            byteStr += c;
        }
    }
    binaryIP.push_back(std::stoi(byteStr)); // Add the last octet
    
    return binaryIP;
}

int main() {
    std::string ipAddress = "127.0.0.1";
    std::vector<unsigned char> binaryIP = convertIPv4ToBinary(ipAddress);

    // Display the binary representation
    std::cout << "Binary representation of " << ipAddress << ": ";
    for (unsigned char byte : binaryIP) {
        std::cout << std::to_string(byte) << " ";
    }
    std::cout << std::endl;

    return 0;
}

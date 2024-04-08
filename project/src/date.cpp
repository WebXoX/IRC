#include <iostream>
#include <ctime>

int main() {
    // Get current time
    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo = gmtime(&rawtime);
    
    // Format the time
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%a %b %d %Y at %H:%M:%S UTC", timeinfo);
    
    // Output the formatted time
    std::cout << buffer << std::endl;

    return 0;
}

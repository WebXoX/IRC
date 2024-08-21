# IRC
The project entails developing an Internet Relay Chat (IRC) server, referred to as ircserv, using C++ 98. The server must be capable of handling multiple clients simultaneously without hanging and should implement non-blocking I/O operations.

## To run
```
make
``` 

```
./ircserver <port> <password>
```
follow the error cases presented when runing it.
## To connect
- THROUGH NC
```
nc localhost <port>
CAP LS
CAP REQ
CAP END
PASS <PASS>
NICK <NICKNAME>
USER <USERNAME> 
```
- THROUGH irssi CLIENT
```
/connect localhost <port> <pass>
```
### Mandatory Requirements:
- **Program Name:** `ircserv`
- **Files to be Turned In:**
  - `Makefile`
  - All source files (`*.{h, hpp}, *.cpp, *.tpp, *.ipp`)
  - Optional configuration file
- **Makefile Rules:** 
  - `$(NAME)`, `all`, `clean`, `fclean`, `re`
- **Compilation:**
  - Compile with `c++` and the flags `-Wall -Wextra -Werror`
  - Code must comply with the C++ 98 standard.
  - Must still compile with the flag `-std=c++98`.
  - Prefer C++ functions over their C equivalents.
  - External libraries and Boost libraries are forbidden.
- **External Functions:**
  - Functions from the C++ 98 standard library, including socket operations, memory management, file I/O, and signal handling.
  - Specific functions listed include: `socket`, `close`, `setsockopt`, `getsockname`, `getprotobyname`, `gethostbyname`, `getaddrinfo`, `freeaddrinfo`, `bind`, `connect`, `listen`, `accept`, `htons`, `htonl`, `ntohs`, `ntohl`, `inet_addr`, `inet_ntoa`, `send`, `recv`, `signal`, `sigaction`, `lseek`, `fstat`, `fcntl`, `poll` (or equivalent).
- **Libft:**
  - No external libraries are allowed, including Libft.
- **Description:**
  - Develop an IRC server in C++ 98.
  - Do not develop a client or handle server-to-server communication.
  - Server executable to be run as: `./ircserv <port> <password>`

### Specific Functional Requirements:
- **Server Capabilities:**
  - Handle multiple clients simultaneously without hanging.
  - All I/O operations must be non-blocking.
  - Use only one `poll()` (or equivalent) for handling all operations (read, write, listen, etc.).
- **IRC Client Interaction:**
  - Develop a reference client to connect to the server without errors.
  - Communication between client and server via TCP/IP (v4 or v6).
  - Implement IRC functionalities such as authentication, setting nickname/username, joining channels, sending/receiving private messages.
  - Implement channel-specific commands for channel operators (KICK, INVITE, TOPIC, MODE).
- **Code Quality:**
  - Write clean, well-structured code.
  - Adhere to best practices in software development.
  
### Additional Guidelines:
- **Platform-Specific Consideration (For MacOS only):**
  - Use `fcntl()` for non-blocking behavior.
  - Use `fcntl(fd, F_SETFL, O_NONBLOCK);`.
  - Other flags are forbidden.
  
### Bonus Requirements:
- **File Transfer:**
  - Implement file transfer functionality.
- **Bot:**
  - Implement a bot feature.
  
### Testing:
- **Error and Issue Verification:**
  - Verify every possible error and issue, including receiving partial data and low bandwidth.
  - Test with `nc` (netcat) to ensure correct processing of commands.


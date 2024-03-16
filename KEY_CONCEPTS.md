# KEY_CONCEPTS

## Socket Programming

### Stream Socket (TCP):
Think of it like a phone call. It provides a reliable, two-way communication channel where data is sent in a continuous stream, ensuring that the information arrives in the correct order without loss.

- Technology: Typically uses TCP (Transmission Control Protocol).
- Reliability: It ensures that your conversation (data) is reliable and in order. If a word gets missed or misunderstood, the system works to correct it so that the overall message is accurate and complete.
- Connection-Oriented: It's like having a constant, established connection, much like staying on the line during a phone call. You can keep talking without worrying about setting up new connections for every piece of information. TCP establishes the connection between devices before data exchange. It involves a three-way handshake (SYN, SYN-ACK, ACK) to ensure both ends are ready.

### Datagram Socket (UDP):
Picture it like sending a letter. It's a connectionless communication method where data is sent in small, independent packets (datagrams). Each packet is sent individually and may take different paths to reach the destination, so there's no guarantee of order or delivery.

- Technology: Often uses UDP (User Datagram Protocol).
- Unreliable: Provides a best-effort delivery, but there's no guarantee of delivery or order. UDP doesn't perform retransmission of lost packets or manage flow control.
- Independence: Each piece of information (datagram) is independent, like individual letters. They may take different routes to reach the destination, and there's no guarantee they'll arrive in the same order they were sent.
- No Connection: Unlike a continuous phone call, there's no persistent connection. Each letter (datagram) is sent separately without maintaining an ongoing link. It's more like tossing messages into the postal system and hoping they reach the destination.
- Fast: Due to its simplicity, UDP is often faster than TCP. It's suitable for real-time applications like video streaming or online gaming, where slight delays are acceptable, and speed is crucial.

## Byte Order

### Big Endian:
Think of it like writing numbers from left to right, with the most significant digit (the big end) on the left. For example, in the decimal number 1234, "1" is the most significant digit.

- In Memory: The most significant byte is stored at the lowest memory address.

### Little Endian:
Imagine writing numbers from right to left, with the least significant digit (the little end) on the left. In the decimal number 1234, "4" is the least significant digit.

- In Memory: The least significant byte is stored at the lowest memory address.

---

In essence, it's about the order in which bytes are stored in memory. Big Endian starts with the most significant byte, while Little Endian starts with the least significant byte.

## Network Byte Order:
Network protocols, including the Internet Protocol (IP) and Transmission Control Protocol (TCP), commonly use big endian (also known as network byte order) for representing multi-byte data types.
This means that when you send data over the network, you should arrange the bytes in big endian order.

### Little Endian Processors:
Many modern processors use little endian byte order for their internal representation of multi-byte values.
When you need to send data from a little endian processor to a network (which expects big endian), you need to perform byte order conversion.
- Byte Order Conversion:
Before sending data over the network, you convert the data from the native little endian format to the big endian format.
Similarly, when receiving data from the network, you convert it from big endian to the little endian format before processing it on a little endian machine.

This byte order conversion is often handled by specific functions or libraries provided by programming languages or operating systems. For example, in C, the functions htons (host to network short) and htonl (host to network long) are commonly used to convert values to network byte order before sending them over the network. The reverse functions, ntohs and ntohl, are used to convert values received from the network to the host byte order.

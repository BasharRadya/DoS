#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int send_UDP_packet() {
    // Create a UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    // Destination address and port (replace with the actual destination IP and port)
    std::string destinationIP = "185.169.134.67"; // Replace with the destination IP address
    int destinationPort = 7777; // Replace with the destination port number

    // Prepare the destination address structure
    struct sockaddr_in destAddr;
    memset(&destAddr, 0, sizeof(destAddr));
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(destinationPort);
    inet_pton(AF_INET, destinationIP.c_str(), &(destAddr.sin_addr));

    // Data to be sent in the UDP packet
    std::string data = "Hello, UDP!";

    // Send the UDP packet
    int bytesSent = sendto(sockfd, data.c_str(), data.size(), 0, (struct sockaddr*)&destAddr, sizeof(destAddr));
    if (bytesSent < 0) {
        std::cerr << "Error sending data" << std::endl;
        close(sockfd);
        return 1;
    } else {
        std::cout << "Sent " << bytesSent << " bytes: UDP Packet" << std::endl;
    }

    // Close the socket
    close(sockfd);

    return 0;
}
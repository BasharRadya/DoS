#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// Simple checksum function for the ICMP packet
unsigned short checksum(void *b, int len) {
    unsigned short *buf = static_cast<unsigned short *>(b);
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2) {
        sum += *buf++;
    }
    if (len == 1) {
        sum += *(unsigned char *)buf;
    }

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;

    return result;
}

int send_ping_packet() {
    // Create a raw socket
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    // Destination address (replace with the random server's IP)
    std::string destinationIP = "185.169.134.67"; // Replace with the target IP address

    // Prepare the destination address structure
    struct sockaddr_in destAddr;
    memset(&destAddr, 0, sizeof(destAddr));
    destAddr.sin_family = AF_INET;
    inet_pton(AF_INET, destinationIP.c_str(), &(destAddr.sin_addr));

    int numPackets = 5; // Number of ICMP ping packets to send

    for (int i = 0; i < 1; ++i) {
        // ICMP packet
        char packet[sizeof(struct icmphdr)];
        struct icmphdr *icmpHeader = reinterpret_cast<struct icmphdr *>(packet);

        // Fill the ICMP packet
        icmpHeader->type = ICMP_ECHO;
        icmpHeader->code = 0;
        icmpHeader->checksum = 0;
        icmpHeader->un.echo.id = htons(getpid());
        icmpHeader->un.echo.sequence = htons(1);
        icmpHeader->checksum = checksum(icmpHeader, sizeof(struct icmphdr));

        // Send the ICMP ping packet
        int bytesSent = sendto(sockfd, packet, sizeof(packet), 0, (struct sockaddr *) &destAddr, sizeof(destAddr));
        if (bytesSent < 0) {
            std::cerr << "Error sending data" << std::endl;
            close(sockfd);
            return 1;
        } else {
            std::cout << "Sent " << bytesSent << " bytes: ICMP Ping" << std::endl;
        }

        // Buffer to receive incoming messages
        char buffer[4096];

        // Wait for ICMP echo replies
        struct sockaddr_in srcAddr;
        socklen_t srcAddrLen = sizeof(srcAddr);
        int bytesRead = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &srcAddr, &srcAddrLen);
        if (bytesRead < 0) {
            std::cerr << "Error receiving data" << std::endl;
            close(sockfd);
            return 1;
        } else {
            std::cout << "Received " << bytesRead << " bytes: ICMP Reply" << std::endl;
            // You can process the received ICMP reply here if needed
        }

    }
    // Close the socket
    close(sockfd);

    return 0;
}
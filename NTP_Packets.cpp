#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <chrono>

// NTP packet structure
struct NtpPacket {
    uint8_t li_vn_mode;      // Leap Indicator, Version Number, and Mode
    uint8_t stratum;         // Stratum level of the local clock
    uint8_t poll;            // Poll interval
    int8_t precision;        // Precision of the local clock
    uint32_t rootDelay;      // Root delay between the server and the reference clock
    uint32_t rootDispersion; // Root dispersion (maximum error)
    uint32_t refId;          // Reference ID
    uint32_t refTm_s;        // Reference timestamp (seconds)
    uint32_t refTm_f;        // Reference timestamp (fractions of a second)
    uint32_t origTm_s;       // Originate timestamp (seconds)
    uint32_t origTm_f;       // Originate timestamp (fractions of a second)
    uint32_t rxTm_s;         // Receive timestamp (seconds)
    uint32_t rxTm_f;         // Receive timestamp (fractions of a second)
    uint32_t txTm_s;         // Transmit timestamp (seconds)
    uint32_t txTm_f;         // Transmit timestamp (fractions of a second)
};
// NTP packet structure (same as previous example)

// Function to send NTP packets continuously
void sendNtpPackets(const std::string& destinationIP, int destinationPort) {
    while (true) {
        int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0) {
            std::cerr << "Error creating socket" << std::endl;
            return;
        }

        struct sockaddr_in destAddr;
        memset(&destAddr, 0, sizeof(destAddr));
        destAddr.sin_family = AF_INET;
        destAddr.sin_port = htons(destinationPort);
        inet_pton(AF_INET, destinationIP.c_str(), &(destAddr.sin_addr));

        NtpPacket ntpPacket;
        memset(&ntpPacket, 0, sizeof(ntpPacket));
        ntpPacket.li_vn_mode = 0b00100011;
        ntpPacket.txTm_s = htonl(time(NULL) + 2208988800);

        int bytesSent = sendto(sockfd, (char*)&ntpPacket, sizeof(ntpPacket), 0, (struct sockaddr*)&destAddr, sizeof(destAddr));
        if (bytesSent < 0) {
            std::cerr << "Error sending data" << std::endl;
        } else {
            std::cout << "Sent " << bytesSent << " bytes: NTP Packet" << std::endl;
        }

        close(sockfd);

        // Wait for a short interval before sending the next NTP packet
        //std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int send_NTP_Packets() {
    std::string destinationIP = "185.169.134.67"; // Replace with the NTP server address
    int destinationPort = 123;

    // Create a separate thread for continuous NTP packet sending
    std::thread sendThread(sendNtpPackets, destinationIP, destinationPort);

    // Optionally, you can join the thread to wait for it to finish,
    // or you can detach the thread to let it run independently.
    // sendThread.join();
    sendThread.detach();

    // The main thread can continue with other tasks or simply sleep
    std::this_thread::sleep_for(std::chrono::seconds(60));

    return 0;
}































/*
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// NTP packet structure
struct NtpPacket {
    uint8_t li_vn_mode;      // Leap Indicator, Version Number, and Mode
    uint8_t stratum;         // Stratum level of the local clock
    uint8_t poll;            // Poll interval
    int8_t precision;        // Precision of the local clock
    uint32_t rootDelay;      // Root delay between the server and the reference clock
    uint32_t rootDispersion; // Root dispersion (maximum error)
    uint32_t refId;          // Reference ID
    uint32_t refTm_s;        // Reference timestamp (seconds)
    uint32_t refTm_f;        // Reference timestamp (fractions of a second)
    uint32_t origTm_s;       // Originate timestamp (seconds)
    uint32_t origTm_f;       // Originate timestamp (fractions of a second)
    uint32_t rxTm_s;         // Receive timestamp (seconds)
    uint32_t rxTm_f;         // Receive timestamp (fractions of a second)
    uint32_t txTm_s;         // Transmit timestamp (seconds)
    uint32_t txTm_f;         // Transmit timestamp (fractions of a second)
};

int send_NTP_Packets() {
    // Create a UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    // Destination NTP server address and port
    std::string destinationIP = "185.169.134.67"; // Replace with the NTP server address
    int destinationPort = 123;

    // Prepare the destination address structure
    struct sockaddr_in destAddr;
    memset(&destAddr, 0, sizeof(destAddr));
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(destinationPort);
    inet_pton(AF_INET, destinationIP.c_str(), &(destAddr.sin_addr));

    // Create and fill the NTP packet
    NtpPacket ntpPacket;
    memset(&ntpPacket, 0, sizeof(ntpPacket));
    ntpPacket.li_vn_mode = 0b00100011; // NTP version 4, mode 3 (client)
    ntpPacket.txTm_s = htonl(time(NULL) + 2208988800); // Transmit timestamp (current time in NTP format)

    // Send the NTP packet
    int bytesSent = sendto(sockfd, (char*)&ntpPacket, sizeof(ntpPacket), 0, (struct sockaddr*)&destAddr, sizeof(destAddr));
    if (bytesSent < 0) {
        std::cerr << "Error sending data" << std::endl;
        close(sockfd);
        return 1;
    } else {
        std::cout << "Sent " << bytesSent << " bytes: NTP Packet" << std::endl;
    }

    // Close the socket
    close(sockfd);

    return 0;
}
 */
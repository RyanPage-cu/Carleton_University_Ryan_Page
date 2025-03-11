
/**
 * @file IntermediateHost.cpp
 * @author Ryan Page
 * @date 2/06/2025
 * Last Updated: 2/08/2025
 */

#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <sstream>
#include "Datagram.h"

//defines the port number for the intermediate host and the server
#define INTERMEDIATE_PORT 23
#define SERVER_PORT 69
/**
 * @class IntermediateHost
 * @brief A class that acts as an intermediate host to forward packets between a client and a server.
 */
class IntermediateHost {
public:
    void Run_IntermediateHost() {
        DatagramSocket clientSocket(INTERMEDIATE_PORT);
        DatagramSocket send_receiveSocket;
        while (true) {
            std::vector<uint8_t> buffer(1024);
            DatagramPacket packet(buffer, buffer.size());

            clientSocket.receive(packet);
            std::cout << "Intermediate Host received request" << std::endl;

            std::vector<uint8_t> data = printPacketData(packet);
            
            // Forward packet to the server
            DatagramPacket forwardPacket(data, packet.getLength(), InetAddress::getLocalHost(), SERVER_PORT);
            send_receiveSocket.send(forwardPacket);
            std::cout << "Intermediate Host sent request to server (Port 69)" << std::endl;

            // Receive response from the server
            std::vector<uint8_t> response(1024);
            DatagramPacket responsePacket(response, response.size());
            send_receiveSocket.receive(responsePacket);
            std::cout << "Intermediate Host received response from server" << std::endl;

            std::vector<uint8_t> data_response = printPacketData(responsePacket);

            // Forward server response to the client
            DatagramPacket clientResponsePacket(data_response, responsePacket.getLength(), packet.getAddress(), packet.getPort());
            clientSocket.send(clientResponsePacket);
            std::cout << "Intermediate Host sent response to client" << std::endl;
        }
    }

private:
    std::vector<uint8_t> printPacketData(DatagramPacket& packet) {
        std::vector<uint8_t> packetData;
        packetData.assign(static_cast<uint8_t*>(packet.getData()),
                            static_cast<uint8_t*>(packet.getData()) + packet.getLength());
        std::ostringstream oss;
        oss << "Packet Data as String: ";
        for (auto byte : packetData) {
            if (byte >= 32 && byte <= 126) {
                oss << static_cast<char>(byte);
            } else {
                oss << "[" << static_cast<int>(byte) << "]";
            }
        }
        std::cout << oss.str() << std::endl;

        std::cout << "Packet Data as Bytes: ";
        for (auto b : packetData) {
            std::cout << static_cast<int>(b) << " ";
        }
        std::cout << "\n";

        return packetData;
    }
};

int main() {
    IntermediateHost intermediateHost;
    intermediateHost.Run_IntermediateHost();
    return 0;
}

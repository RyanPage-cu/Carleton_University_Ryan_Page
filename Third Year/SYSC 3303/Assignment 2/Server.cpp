
/**
 * @file Server.cpp
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

//defines the port number for the server
#define SERVER_PORT 69
/**
 * @class Server
 * @brief A class that acts as an server, reciving packets from a client, printing the packet data, and sending a response back to the client of weather the packet was read or wirte.
 */
class Server {
public:
    void Run_Server() {
        DatagramSocket socket(SERVER_PORT);
        while (true) {
            std::vector<uint8_t> buffer(1024);
            DatagramPacket packet(buffer, buffer.size());

            socket.receive(packet);
            std::cout << "Server received request" << std::endl;

            printPacketData(packet);
            if (isValidRequest(packet)) {
                std::vector<uint8_t> response;
                std::vector<uint8_t> data;
                data.assign(static_cast<uint8_t*>(packet.getData()), static_cast<uint8_t*>(packet.getData()) + packet.getLength());

                // Constructing a response based on the request type
                if (data[1] == 1) {
                    response = {0, 3, 0, 0};  // Read request response
                } else if (data[1] == 2) {
                    response = {0, 4, 0, 0};  // Write request response
                }

                std::cout << "Server constructed response" << std::endl;
                DatagramPacket responsePacket(response, response.size(), packet.getAddress(), packet.getPort());
                DatagramSocket responseSocket;
                printPacketData(responsePacket);
                responseSocket.send(responsePacket);
                std::cout << "Server sent response" << std::endl;
            } else {
                throw std::runtime_error("Received invalid request");
            }
        }
    }
private:
    void printPacketData(DatagramPacket& packet) {
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
    }

    bool isValidRequest(DatagramPacket& packet) {
        std::vector<uint8_t> data;
        data.assign(static_cast<uint8_t*>(packet.getData()), static_cast<uint8_t*>(packet.getData()) + packet.getLength());

        // Validate the request based on the first two bytes
        if (data[0] != 0 || (data[1] != 1 && data[1] != 2)) {
            return false;
        }

        return true;
    }

};

int main() {
    Server server;
    server.Run_Server();
    return 0;
}




/**
 * @file Client.cpp
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

//defines the port number for the intermediate host
#define INTERMEDIATE_PORT 23
/**
 * @class Client
 * @brief A class that acts as the client, in the client server relationship, sending packets that should be recived by the server but has to go through an intermediate host to get there.
 */
class Client {
public:
    void Client_run() {
        DatagramSocket socket;
        for (int i = 1; i < 12; i++) {
            std::cout << "Request #" << i << std::endl;
            int8_t read_or_write;
            std::vector<uint8_t> data;
            if (i == 11) {
                read_or_write = 0;
                data.push_back(0);
                data.push_back(0);
            } else if (i % 2 == 0) {
                read_or_write = 1;
                data.push_back(0);
                data.push_back(1);
            } else if (i % 2 == 1) {
                read_or_write = 2;
                data.push_back(0);
                data.push_back(2);
            }
            
            std::string filename = "file" + std::to_string(i) + ".txt";
            data.insert(data.end(), filename.begin(), filename.end());
            data.push_back(0);
            std::string mode = "ocTEt";
            data.insert(data.end(), mode.begin(), mode.end());
            data.push_back(0);

            std::cout << "------------------------------------------------------------------------------------------------------------------------" << std::endl;
            std::cout << "Client sending request" << std::endl;

            

            DatagramPacket packet(data, data.size(), InetAddress::getLocalHost(), INTERMEDIATE_PORT);
            printPacketData(packet);
            socket.send(packet);
            
            std::vector<uint8_t> response(1024);
            DatagramPacket responsePacket(response, response.size());
            socket.receive(responsePacket);
            std::cout << "Client received response" << std::endl;
            printPacketData(responsePacket);
            std::cout << "------------------------------------------------------------------------------------------------------------------------" << std::endl;
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
};

int main() {
    Client client;
    client.Client_run();
    return 0;
}

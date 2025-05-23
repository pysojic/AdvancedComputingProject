#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>
#include <arpa/inet.h>
#include "../include/OrderBook.hpp"

#define MAX_UDP_PAYLOAD 1400 // Matches the server's max UDP payload
#define CLIENT_UDP_PORT 3001 // Match this with the server's sending port
#define TCP_PORT 4000
#define LOCAL_IP "127.0.0.1"

void parse_message(const std::string& message, OrderBook& order_book, int& challenge_id, std::string& target_sec) {
    std::istringstream stream(message);
    std::string line;
    std::string sec_id;
    double bid, ask;

    while (std::getline(stream, line)) {
        if (line.empty()) continue;
        std::istringstream line_stream(line);
        std::string token;
        std::vector<std::string> tokens;

        if (line.find('|') != std::string::npos) {
            while (std::getline(line_stream, token, '|')) {
                tokens.push_back(token);
            }

            if (tokens.size() == 6 && tokens[0] == "SEC") {
                sec_id = tokens[1];
                bid = std::stod(tokens[3]);
                ask = std::stod(tokens[5]);
                order_book.addOrder(sec_id, bid, ask);
            }
        } 
        else {
            while (std::getline(line_stream, token, ':')) {
                tokens.push_back(token);
            }
            
            if (tokens[0] == "TARGET") {
                target_sec = tokens[1];
            }
            else {
                challenge_id = std::stoi(tokens[1]);
            }
        }
    }
}

int main() {
    // Set UDP socket
    int client_fd_udp = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_fd_udp < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Allow reuse to prevent conflicts
    int enable_reuse = 1;
    setsockopt(client_fd_udp, SOL_SOCKET, SO_REUSEADDR, &enable_reuse, sizeof(enable_reuse));

    sockaddr_in client_addr_udp{};
    client_addr_udp.sin_family = AF_INET;
    client_addr_udp.sin_port = htons(CLIENT_UDP_PORT);
    client_addr_udp.sin_addr.s_addr = inet_addr(LOCAL_IP);

    if (bind(client_fd_udp, (sockaddr*)&client_addr_udp, sizeof(client_addr_udp)) < 0) {
        perror("UDP Binding failed");
        return 1;
    }

    std::cout << "UDP Client listening on port " << CLIENT_UDP_PORT << "...\n";

    // Set up TCP socket
    int client_fd_tcp = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_addr_tcp = {AF_INET, 0, INADDR_ANY};
    server_addr_tcp.sin_port = htons(TCP_PORT);

    connect(client_fd_tcp, (sockaddr*)&server_addr_tcp, sizeof(server_addr_tcp)); // Connect to server
    std::cout << "TCP connection successfully established!" << std::endl;

    char buffer[MAX_UDP_PAYLOAD];
    socklen_t client_len = sizeof(client_addr_udp);

    std::string full_message;

    while (true) {
        ssize_t received = recvfrom(client_fd_udp, buffer, sizeof(buffer), 0, (sockaddr*)&client_addr_udp, &client_len);
        if (received < 0) {
            perror("recvfrom failed");
            break;
        }

        std::string fragment(buffer, received);
        full_message += fragment;

        if (received < MAX_UDP_PAYLOAD) {
            // std::cout << "Message received: " << full_message << std::endl;
            OrderBook order_book;
            int challenge_id;
            std::string target_sec;
            parse_message(full_message, order_book, challenge_id, target_sec);
            std::cout << "Challenge ID: " << challenge_id << "\n";
            std::cout << "Target Security: " << target_sec << "\n";
            order_book.print_book();
            std::cout << "\n\n";

            std::string response = "CHALLENGE_RESPONSE " + std::to_string(challenge_id) + " " + target_sec + 
                                   " BID: " + std::to_string(std::get<0>(order_book.queryTicker(target_sec))) +
                                   " ASK: " + std::to_string(std::get<0>(order_book.queryTicker(target_sec))) +
                                   " trader_name" + "\n";


            std::cout << "Sending response to server: " << response << std::endl;
            send(client_fd_tcp, response.c_str(), response.size(), 0);
            std::cout << "Response sent" << std::endl;
            

            full_message.clear();  // Ready for the next message
        }
    }

    close(client_fd_tcp);
    close(client_fd_udp);
    return 0;
}
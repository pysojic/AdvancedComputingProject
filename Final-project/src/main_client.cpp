#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>
#include <arpa/inet.h>
#include <StopWatch.hpp>

#define MAX_UDP_PAYLOAD 1400 // Matches the server's max UDP payload
#define CLIENT_UDP_PORT 3001 // Match this with the server's sending port
#define TCP_PORT 4000
#define LOCAL_IP "127.0.0.1"

int main()
{
    // --- UDP setup ---
    int client_fd_udp = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_fd_udp < 0) {
        perror("Socket creation failed");
        return 1;
    }
    int enable_reuse = 1;
    setsockopt(client_fd_udp, SOL_SOCKET, SO_REUSEADDR,
               &enable_reuse, sizeof(enable_reuse));

    sockaddr_in client_addr_udp{};
    client_addr_udp.sin_family      = AF_INET;
    client_addr_udp.sin_port        = htons(CLIENT_UDP_PORT);
    client_addr_udp.sin_addr.s_addr = inet_addr(LOCAL_IP);

    if (bind(client_fd_udp,
             (sockaddr*)&client_addr_udp,
             sizeof(client_addr_udp)) < 0) {
        perror("UDP Binding failed");
        return 1;
    }
    std::cout << "UDP Client listening on port "
              << CLIENT_UDP_PORT << "...\n";

    // --- TCP setup ---
    int client_fd_tcp = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd_tcp < 0) {
        perror("TCP socket creation failed");
        return 1;
    }
    sockaddr_in server_addr_tcp{};
    server_addr_tcp.sin_family      = AF_INET;
    server_addr_tcp.sin_port        = htons(TCP_PORT);
    server_addr_tcp.sin_addr.s_addr = INADDR_ANY;

    if (connect(client_fd_tcp,
                (sockaddr*)&server_addr_tcp,
                sizeof(server_addr_tcp)) < 0) {
        perror("TCP connection failed");
        return 1;
    }
    std::cout << "TCP connection successfully established!\n";


    char        buffer[MAX_UDP_PAYLOAD];
    socklen_t   client_len   = sizeof(client_addr_udp);
    std::string full_message;

    while (true) 
    {
        StopWatch sw;
        sw.Start();
        ssize_t received = recvfrom(client_fd_udp,
                                    buffer,
                                    sizeof(buffer),
                                    0,
                                    (sockaddr*)&client_addr_udp,
                                    &client_len);
        if (received < 0) 
        {
            perror("recvfrom failed");
            break;
        }

        full_message.append(buffer, received);

        if (received < MAX_UDP_PAYLOAD) {
            

            while (!full_message.empty() && full_message.back() == '\n') 
            {
                full_message.pop_back();
            }
        
            auto pos_last = full_message.rfind('\n');
            auto pos_second_last = (pos_last == std::string::npos)
                ? std::string::npos
                : full_message.rfind('\n', pos_last - 1);
        
            std::string challenge_line = full_message.substr(
                pos_second_last + 1,
                pos_last - pos_second_last - 1
            );
            std::string target_line = full_message.substr(pos_last + 1);
            
            std::string challenge_id = challenge_line.substr(challenge_line.find(':') + 1);
            std::string target_sec   = target_line.substr(target_line.find(':') + 1);

            std::string target_bid, target_ask;
            std::string prefix = "SEC|" + target_sec + "|BID|";

            auto pos = full_message.find(prefix);
            if (pos != std::string::npos) {
                size_t bid_start = pos + prefix.size();
                size_t bid_end   = full_message.find('|', bid_start);
                if (bid_end != std::string::npos) {
                    target_bid = full_message.substr(bid_start, bid_end - bid_start);

                    const std::string ask_prefix = "|ASK|";
                    auto ask_pos = full_message.find(ask_prefix, bid_end);
                    if (ask_pos != std::string::npos) {
                        size_t ask_start = ask_pos + ask_prefix.size();
                        size_t ask_end   = full_message.find_first_of("\r\n", ask_start);
                        target_ask = full_message.substr(
                            ask_start,
                            (ask_end == std::string::npos ? 
                            full_message.size() : ask_end) - ask_start
                        );
                    }
                }
            }

            std::cout << "CHALLENGE_ID: " << challenge_id << "\n";
            std::cout << "TARGET:       " << target_sec  << "\n";
            std::cout << "BID (regex):  " << target_bid  << "\n";
            std::cout << "ASK (regex):  " << target_ask  << "\n";
            std::cout << "\n";

            std::string response =
                "CHALLENGE_RESPONSE " + challenge_id +
                " " + target_sec +
                " BID: " + target_bid +
                " ASK: " + target_ask +
                " trader_name\n";

            send(client_fd_tcp,
                 response.c_str(),
                 response.size(),
                 0);

            sw.Stop();
            sw.display_time();
            std::cout << "Sent: " << response << "\n";

            full_message.clear();
        }
    }

    close(client_fd_tcp);
    close(client_fd_udp);
    return 0;
}
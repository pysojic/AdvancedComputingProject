#include <iostream>
#include <thread>
#include <chrono>
#include <cstring>
#include <deque>
#include <cstdlib>
#include <format>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define BUFFER_SIZE 1024

std::pair<size_t, size_t> receiveAndRespond(int socketFd, const string& name) 
{
    char buffer[BUFFER_SIZE];
    std::deque<float> priceHistory;
    size_t hits = 0;
    size_t orderCount = 0; // Count for all orders (hit and non-hit)

    // Send client name
    send(socketFd, name.c_str(), name.size(), 0);

    while (true) 
    {
        memset(buffer, 0, BUFFER_SIZE);
        int bytesReceived = recv(socketFd, buffer, BUFFER_SIZE - 1, 0);
        if (bytesReceived <= 0) 
        {
            cerr << "Server closed connection or error occurred." << endl;
            break;
        }

        string data(buffer);
        size_t commaPos = data.find(',');
        if (commaPos == string::npos) 
        {
            cerr << "Invalid price format received: " << data << endl;
            continue;
        }

        int priceId = stoi(data.substr(0, commaPos));
        float price = stof(data.substr(commaPos + 1));

        if (priceHistory.size() >= 3)
            priceHistory.pop_front();
        priceHistory.push_back(price);

        cout << "📥 Received price ID: " << priceId << ", Value: " << price << endl;

        if (priceHistory.size() == 3) 
        {
            float a = priceHistory[0];
            float b = priceHistory[1];
            float c = priceHistory[2];
        
            bool up = (a < b) && (b < c);
            bool down = (a > b) && (b > c);
        
            if (up || down) 
            {
                this_thread::sleep_for(chrono::milliseconds(10 + rand() % 50));
                // Send order (price ID)
                string order = to_string(priceId);
                send(socketFd, order.c_str(), order.length(), 0);
                cout << "Momentum up! Sending order for price ID " << priceId << endl;
                ++hits;
            }
            else 
            {
                cout << "No momentum. Ignoring price ID" << priceId << endl;
            }
        }

        ++orderCount;

        // Simulate reaction delay
        this_thread::sleep_for(chrono::milliseconds(100 + rand() % 300));
    }

    close(socketFd);

    return {orderCount, hits};
}

int main() {
    srand(time(nullptr));

    string name;
    cout << "Enter your client name: ";
    getline(cin, name);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        cerr << "Socket creation failed!" << endl;
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        cerr << "Connection to server failed!" << endl;
        return 1;
    }

    cout << "✅ Connected to server at " << SERVER_IP << ":" << SERVER_PORT << endl;
    auto stats = receiveAndRespond(sock, name);

    cout << std::format("Total number of orders received: {}\n Total number of hits: {}\n Hit-to-order ratio: {}%",
        stats.first, stats.second, static_cast<float>(stats.second) / stats.first * 100);
    return 0;
}

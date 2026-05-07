#include "server.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <cstring>

#define PORT 5000

struct Client {
    int socket;
    char id;
};

std::vector<Client> clients;
std::mutex clientsMutex;


/**
 * @brief 
 * 
 * @param client_fd 
 */
void handleClient(int client_fd) {
    char buffer[1024];

    int n = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (n <= 0) {
        close(client_fd);
        return;
    }

    buffer[n] = '\0';
    char id = buffer[0];

    {
        std::lock_guard<std::mutex> lock(clientsMutex);
        clients.push_back({client_fd, id});
    }

    std::cout << "Wemos [" << id << "] connected ?\n";

    while (true) {
        n = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

        if (n <= 0) {
            std::cout << "Wemos [" << id << "] disconnected ?\n";
            break;
        }

        buffer[n] = '\0';
        std::string msg(buffer);

        if (!msg.empty() && msg.back() == '\n') {
            msg.pop_back();
        }

        size_t pos = msg.find(';');
        if (pos == std::string::npos) continue;

        std::string value = msg.substr(pos + 1);

        size_t spacePos = value.find(' ');
        if (spacePos == std::string::npos) continue;

        std::string type  = value.substr(0, spacePos);
        std::string state = value.substr(spacePos + 1);

        std::cout << "Wemos [" << id << "] "
                  << type << ": " << state << std::endl;
    }

    close(client_fd);
}

/**
 * @brief 
 * 
 */
void commandLoop() {
    while (true) {
        char target, cmd;

        std::cout << "\nTarget (A/B/C): ";
        std::cin >> target;

        std::cout << "Command (1=blink, 2=off): ";
        std::cin >> cmd;

        std::lock_guard<std::mutex> lock(clientsMutex);

        for (auto &c : clients) {
            if (c.id == target) {
                send(c.socket, &cmd, 1, 0);
                std::cout << "? Sent to " << target << "\n";
            }
        }
    }
}


/**
 * @brief 
 * 
 */
void startServer() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 5);

    std::cout << "Server running on port " << PORT << "...\n";

    std::thread cmdThread(commandLoop);
    cmdThread.detach();

    while (true) {
        int client_fd = accept(server_fd, nullptr, nullptr);
        std::thread(handleClient, client_fd).detach();
    }

    close(server_fd);
}

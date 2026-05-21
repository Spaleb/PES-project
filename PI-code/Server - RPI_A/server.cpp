#include "server.h"

#include <arpa/inet.h>
#include <unistd.h>

#include <cstring>
#include <mutex>
#include <thread>
#include <vector>
#include <iostream>

using std::lock_guard;
using std::mutex;
using std::thread;
using std::vector;

#define WEMOS_PORT 5000
#define PI_PORT 8080
#define PI_IP "145.52.127.228"
#define BUFFER_SIZE 1024

struct Client {
    int socket;
    char id;
};

vector<Client> clients;
mutex clientsMutex;

int sock = 0;
sockaddr_in serv_addr = {};

void sendToPi(const std::string& msg) {
    if (sock <= 0) {
        std::cerr << "Pi socket not connected\n";
        return;
    }

    std::string fullMsg = msg;

    if (fullMsg.back() != '\n') {
        fullMsg += "\n";
    }

    send(sock, fullMsg.c_str(), fullMsg.size(), 0);
}

void receivedWemos(int client_fd) {
    char buffer[BUFFER_SIZE];

    int n = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
    if (n <= 0) {
        close(client_fd);
        return;
    }

    buffer[n] = '\0';
    char id = buffer[0];

    {
        lock_guard<mutex> lock(clientsMutex);

        bool found = false;
        for (auto& c : clients) {
            if (c.id == id) {
                close(c.socket);
                c.socket = client_fd;
                found = true;
                break;
            }
        }

        if (!found) {
            clients.push_back({client_fd, id});
        }

        printf("Client %c %s\n", id, found ? "reconnected" : "connected");
    }

    while (true) {
        n = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
        if (n <= 0) {
            printf("Client %c disconnected\n", id);
            break;
        }

        buffer[n] = '\0';

        bool onlyWhitespace = true;
        for (int i = 0; i < n; ++i) {
            if (!isspace((unsigned char)buffer[i])) {
                onlyWhitespace = false;
                break;
            }
        }

        if (!onlyWhitespace) {
            printf("Received from %c: %s\n", id, buffer);
        
            std::string msg = std::string(buffer);   
            sendToPi(msg);
        }
    }

    close(client_fd);
}

void handleWemos() {
    while (true) {
        char target = 0;
        printf("\nTarget (A/B/C): ");
        scanf(" %c", &target);

        printf("Type bericht (bijv: 1 of ALARM): ");
        char msg_buf[BUFFER_SIZE];
        if (!fgets(msg_buf, BUFFER_SIZE, stdin)) {
            continue;
        }
        int len = strlen(msg_buf);
        if (len > 0 && msg_buf[len - 1] == '\n') msg_buf[--len] = '\0';

        std::string out = std::string(msg_buf) + "\n";

        {
            lock_guard<mutex> lock(clientsMutex);

            for (auto& c : clients) {
                if (c.id == target) {
                    send(c.socket, out.c_str(), out.size(), 0);
                    break;
                }
            }
        }
    }
}

void startPi() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Socket error\n";
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PI_PORT);

    if (inet_pton(AF_INET, PI_IP, &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid PI IP\n";
        return;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Pi connection failed\n";
        return;
    }
    std::cout << "Connected to Pi!\n";
}

void initWemos() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in address = {};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(WEMOS_PORT);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 5);

    while (true) {
        int client_fd = accept(server_fd, nullptr, nullptr);
        thread(receivedWemos, client_fd).detach();
    }
}    char id = buffer[0];

    {
        lock_guard<mutex> lock(clientsMutex);
        bool found = false;

        for (auto& c : clients) {
            if (c.id == id) {
                close(c.socket);
                c.socket = client_fd;
                found = true;
                break;
            }
        }

        if (!found) {
            clients.push_back({client_fd, id});
        }

        printf("Client %c %s\n", id, found ? "reconnected" : "connected");
    }

    while (true) {
        n = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
        if (n <= 0) {
            printf("Client %c disconnected\n", id);
            break;
        }

        buffer[n] = '\0';

        bool onlyWhitespace = true;
        for (int i = 0; i < n; ++i) {
            if (!isspace((unsigned char)buffer[i])) {
                onlyWhitespace = false;
                break;
            }
        }

        if (!onlyWhitespace) {
            printf("Received from %c: %s\n", id, buffer);
        }
    }

    close(client_fd);
}

/**
 * @brief Process console commands and send messages to the selected client.
 *
 * This loop reads a target ID and message from stdin and sends the message
 * only to the client with that ID.
 */
void commandLoop() {
    while (true) {
        char target = 0;
        usleep(100000);
        printf("\nTarget (A/B/C): ");
        scanf(" %c", &target);

        printf("Type bericht (bijv: 1 of ALARM): ");
        char msg_buf[BUFFER_SIZE];
        if (!fgets(msg_buf, BUFFER_SIZE, stdin)) {
            continue;
        }

        int len = strlen(msg_buf);
        if (len > 0 && msg_buf[len - 1] == '\n') {
            msg_buf[--len] = '\0';
        }

        if (len < BUFFER_SIZE - 1) {
            msg_buf[len++] = '\n';
            msg_buf[len] = '\0';
        }

        lock_guard<mutex> lock(clientsMutex);
        for (auto& c : clients) {
            if (c.id == target) {
                send(c.socket, msg_buf, len, 0);
            }
        }
    }
}

/**
 * @brief Start the TCP server and accept incoming client connections.
 *
 * This function creates a listening socket, starts the console command loop
 * in a separate thread, and then accepts new clients in an infinite loop.
 */
void startServer() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in address = {};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 5);

    thread(commandLoop).detach();

    while (true) {
        int client_fd = accept(server_fd, nullptr, nullptr);
        thread(handleClient, client_fd).detach();
    }
}

#include "server.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdio>
#include <cctype>
#include <cstring>
#include <mutex>
#include <thread>
#include <vector>

using std::lock_guard;
using std::mutex;
using std::thread;
using std::vector;

/**
 * @file server.cpp
 * @brief Implementation of the PI-Wemos TCP server.
 */

/**
 * @brief TCP port where the server listens.
 */
#define PORT 5000

/**
 * @brief Size of the buffer for incoming messages.
 */
#define BUFFER_SIZE 1024

/**
 * @brief One client consists of a socket and an ID character.
 */
struct Client {
    int socket; /**< Socket descriptor of the client. */
    char id;    /**< Unique ID character of the client. */
};

/**
 * @brief Stores all connected clients.
 */
vector<Client> clients;

/**
 * @brief Protects access to the client list across threads.
 */
mutex clientsMutex;

/**
 * @brief Process the connection of a single client.
 *
 * This function reads the first message from the client to determine the
 * client ID, stores the client in the global list, and then processes
 * incoming messages until the connection is closed.
 *
 * @param client_fd Socket descriptor of the connected client.
 */
void handleClient(int client_fd) {
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

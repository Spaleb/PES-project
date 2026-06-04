#include "server.h"

#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

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
#define PI_IP "192.168.137.104"
#define BUFFER_SIZE 1024

struct Client {
    int socket;
    char id;
};

vector<Client> clients;
mutex clientsMutex;

int sock = 0;
sockaddr_in serv_addr = {};

void processMessage(const std::string& msg);
void sendToPi(const std::string& msg);

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

        for (int i = 0; i < n; i++) {
            if (!isspace((unsigned char)buffer[i])) {
                onlyWhitespace = false;
                break;
            }
        }

        if (!onlyWhitespace) {
            printf("Received from %c: %s\n", id, buffer);

            std::string msg(buffer);
            sendToPi(msg);
        }
    }

    close(client_fd);

    {
        lock_guard<mutex> lock(clientsMutex);

        for (auto it = clients.begin(); it != clients.end(); ++it) {
            if (it->id == id) {
                clients.erase(it);
                break;
            }
        }
    }
}

void processMessage(const std::string& msg) {
    if (msg.empty()) {
        return;
    }

    char target = msg[0];

    size_t payloadStart = 1;

    if (msg.size() > 1 && (msg[1] == ' ' || msg[1] == ':')) {
        payloadStart = 2;
    }

    std::string bericht = msg.substr(payloadStart);

    if (bericht.empty()) {
        return;
    }

    if (bericht.back() != '\n') {
        bericht += '\n';
    }

    lock_guard<mutex> lock(clientsMutex);

    for (auto& c : clients) {
        if (c.id == target) {
            send(c.socket, bericht.c_str(), bericht.size(), 0);
            return;
        }
    }

    printf("Wemos target %c not found\n", target);
}

void receiveFromPi() {
    char buffer[BUFFER_SIZE];

    while (true) {
        memset(buffer, 0, BUFFER_SIZE);

        int valread = recv(sock, buffer, BUFFER_SIZE - 1, 0);

        if (valread > 0) {
            buffer[valread] = '\0';

            std::cout << "From Pi: " << buffer << std::endl;

            processMessage(buffer);
        }
        else if (valread == 0) {
            std::cout << "Pi disconnected\n";
            break;
        }
        else {
            std::cerr << "Recv error from Pi\n";
            break;
        }
    }
}

void handleWemos() {
    while (true) {
        char target = 0;

        printf("\nTarget (A/B/C): ");
        scanf(" %c", &target);

        getchar();

        printf("Type bericht: ");

        char msg_buf[BUFFER_SIZE];

        if (!fgets(msg_buf, BUFFER_SIZE, stdin)) {
            continue;
        }

        int len = strlen(msg_buf);

        if (len > 0 && msg_buf[len - 1] == '\n') {
            msg_buf[len - 1] = '\0';
        }

        std::string out = std::string(msg_buf) + "\n";

        lock_guard<mutex> lock(clientsMutex);

        for (auto& c : clients) {
            if (c.id == target) {
                send(c.socket, out.c_str(), out.size(), 0);
                break;
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

    thread(receiveFromPi).detach();
}

void initWemos() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0) {
        std::cerr << "Server socket failed\n";
        return;
    }

    int opt = 1;

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in address = {};

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(WEMOS_PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed\n";
        return;
    }

    if (listen(server_fd, 5) < 0) {
        std::cerr << "Listen failed\n";
        return;
    }

    std::cout << "Wemos server started\n";

    while (true) {
        int client_fd = accept(server_fd, nullptr, nullptr);

        if (client_fd < 0) {
            continue;
        }

        thread(receivedWemos, client_fd).detach();
    }
}

void sendToPi(const std::string& msg) {
    if (sock <= 0) {
        std::cerr << "Pi socket not connected\n";
        return;
    }

    if (msg.empty()) {
        return;
    }

    std::string fullMsg = msg;

    if (!fullMsg.empty() && fullMsg.back() != '\n') {
        fullMsg += '\n';
    }

    send(sock, fullMsg.c_str(), fullMsg.size(), 0);
}#include "server.h"

#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

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

void processMessage(const std::string& msg);
void sendToPi(const std::string& msg);

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

        for (int i = 0; i < n; i++) {
            if (!isspace((unsigned char)buffer[i])) {
                onlyWhitespace = false;
                break;
            }
        }

        if (!onlyWhitespace) {
            printf("Received from %c: %s\n", id, buffer);

            std::string msg(buffer);
            sendToPi(msg);
        }
    }

    close(client_fd);

    {
        lock_guard<mutex> lock(clientsMutex);

        for (auto it = clients.begin(); it != clients.end(); ++it) {
            if (it->id == id) {
                clients.erase(it);
                break;
            }
        }
    }
}

void processMessage(const std::string& msg) {
    if (msg.empty()) {
        return;
    }

    char target = msg[0];

    size_t payloadStart = 1;

    if (msg.size() > 1 && (msg[1] == ' ' || msg[1] == ':')) {
        payloadStart = 2;
    }

    std::string bericht = msg.substr(payloadStart);

    if (bericht.empty()) {
        return;
    }

    if (bericht.back() != '\n') {
        bericht += '\n';
    }

    lock_guard<mutex> lock(clientsMutex);

    for (auto& c : clients) {
        if (c.id == target) {
            send(c.socket, bericht.c_str(), bericht.size(), 0);
            return;
        }
    }

    printf("Wemos target %c not found\n", target);
}

void receiveFromPi() {
    char buffer[BUFFER_SIZE];

    while (true) {
        memset(buffer, 0, BUFFER_SIZE);

        int valread = recv(sock, buffer, BUFFER_SIZE - 1, 0);

        if (valread > 0) {
            buffer[valread] = '\0';

            std::cout << "From Pi: " << buffer << std::endl;

            processMessage(buffer);
        }
        else if (valread == 0) {
            std::cout << "Pi disconnected\n";
            break;
        }
        else {
            std::cerr << "Recv error from Pi\n";
            break;
        }
    }
}

void handleWemos() {
    while (true) {
        char target = 0;

        printf("\nTarget (A/B/C): ");
        scanf(" %c", &target);

        getchar();

        printf("Type bericht: ");

        char msg_buf[BUFFER_SIZE];

        if (!fgets(msg_buf, BUFFER_SIZE, stdin)) {
            continue;
        }

        int len = strlen(msg_buf);

        if (len > 0 && msg_buf[len - 1] == '\n') {
            msg_buf[len - 1] = '\0';
        }

        std::string out = std::string(msg_buf) + "\n";

        lock_guard<mutex> lock(clientsMutex);

        for (auto& c : clients) {
            if (c.id == target) {
                send(c.socket, out.c_str(), out.size(), 0);
                break;
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

    thread(receiveFromPi).detach();
}

void initWemos() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0) {
        std::cerr << "Server socket failed\n";
        return;
    }

    int opt = 1;

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in address = {};

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(WEMOS_PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed\n";
        return;
    }

    if (listen(server_fd, 5) < 0) {
        std::cerr << "Listen failed\n";
        return;
    }

    std::cout << "Wemos server started\n";

    while (true) {
        int client_fd = accept(server_fd, nullptr, nullptr);

        if (client_fd < 0) {
            continue;
        }

        thread(receivedWemos, client_fd).detach();
    }
}

void sendToPi(const std::string& msg) {
    if (sock <= 0) {
        std::cerr << "Pi socket not connected\n";
        return;
    }

    if (msg.empty()) {
        return;
    }

    std::string fullMsg = msg;

    if (!fullMsg.empty() && fullMsg.back() != '\n') {
        fullMsg += '\n';
    }

    send(sock, fullMsg.c_str(), fullMsg.size(), 0);
}

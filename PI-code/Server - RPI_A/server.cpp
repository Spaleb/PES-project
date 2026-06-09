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
#define PI_IP "10.42.0.1"
#define BUFFER_SIZE 1024

// Clientstructuur om verbonden Wemos-apparaten bij te houden
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

/**
 * @brief Functie die wordt aangeroepen wanneer een Wemos-apparaat verbinding maakt en berichten verzendt.
 * 
 * @param client_fd Socketbestanddescriptor van de verbonden Wemos-client.
 */
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

        // Controleer of de Wemos al eerder verbonden was en update de socket indien nodig
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

    // Blijf berichten ontvangen van de Wemos totdat deze wordt verbroken
    while (true) {
        n = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);

        if (n <= 0) {
            printf("Client %c disconnected\n", id);
            break;
        }

        buffer[n] = '\0';

        bool onlyWhitespace = true;

        // Controleer of het ontvangen bericht alleen uit witruimtes bestaat
        for (int i = 0; i < n; i++) {
            if (!isspace((unsigned char)buffer[i])) {
                onlyWhitespace = false;
                break;
            }
        }

        // Als het bericht niet alleen uit witruimtes bestaat, log het en stuur het door naar de Pi
        if (!onlyWhitespace) {
            printf("Received from %c: %s\n", id, buffer);

            std::string msg(buffer);
            sendToPi(msg);
        }
    }

    close(client_fd);

    {
        lock_guard<mutex> lock(clientsMutex);

        // Verwijder de client uit de lijst van verbonden clients wanneer deze wordt verbroken
        for (auto it = clients.begin(); it != clients.end(); ++it) {
            if (it->id == id) {
                clients.erase(it);
                break;
            }
        }
    }
}

/**
 * @brief Functie die een ontvangen bericht verwerkt en naar de juiste Wemos stuurt.
 * 
 * @param msg Het ontvangen bericht.
 */
void processMessage(const std::string& msg) {
    if (msg.empty()) {
        return;
    }

    char target = msg[0];

    size_t payloadStart = 1;

    // Controleer of er een scheidingsteken (spatie of dubbele punt) is.
    if (msg.size() > 1 && (msg[1] == ' ' || msg[1] == ':')) {
        payloadStart = 2;
    }

    std::string bericht = msg.substr(payloadStart);

    if (bericht.empty()) {
        return;
    }

    // Zorg ervoor dat het bericht eindigt met een newline, zodat de Wemos het correct kan verwerken.
    if (bericht.back() != '\n') {
        bericht += '\n';
    }

    lock_guard<mutex> lock(clientsMutex);

    // Zoek de juiste Wemos op basis van het target-id en stuur het bericht door
    for (auto& c : clients) {
        if (c.id == target) {
            send(c.socket, bericht.c_str(), bericht.size(), 0);
            return;
        }
    }

    printf("Wemos target %c not found\n", target);
}

/**
 * @brief Functie die berichten ontvangt van de Pi en verwerkt.
 * 
 */
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

/**
 * @brief Functie die de interactie met de Wemos-apparaten beheert.
 * 
 */
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

        // Zoek de juiste Wemos op basis van het target-id en stuurt het bericht door
        for (auto& c : clients) {
            if (c.id == target) {
                send(c.socket, out.c_str(), out.size(), 0);
                break;
            }
        }
    }
}

/**
 * @brief Functie die de verbinding met de Pi start.
 * 
 */
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

/**
 * @brief Functie die de Wemos-server start.
 * 
 */
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

    // Accepteer binnenkomende verbindingen van Wemos-apparaten en start een nieuwe thread voor elke verbinding
    while (true) {
        int client_fd = accept(server_fd, nullptr, nullptr);

        if (client_fd < 0) {
            continue;
        }

        thread(receivedWemos, client_fd).detach();
    }
}

/**
 * @brief Functie die een bericht naar de Pi stuurt.
 * @param msg Het te sturen bericht.
 */
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

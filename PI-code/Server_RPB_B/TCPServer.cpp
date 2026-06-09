#include "TCPServer.h"

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

/**
 * @brief Construeert een nieuw TCPServer::TCPServer object.
 * 
 */
TCPServer::TCPServer() : server_fd(-1), client_fd(-1) {}

/**
 * @brief Functie voor het starten van de TCP-socket. Hierna kunnen clients kunnen
 connecten tot de server.
 * 
 * @param port Poort waarop de clients kunnen verbinden.
 * @return true Als de server correct gestart.
 * @return false Als de server niet correct gestart is.
 */
bool TCPServer::start(int port) {
    sockaddr_in address{};
    int opt = 1;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) return false;

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0)
        return false;

    if (listen(server_fd, 5) < 0)
        return false;

    return true;
}

/**
 * @brief Functie voor het accepteren van een client die wil connecten tot de server.
 * 
 * @return int returned de FD (file descriptor) van de nieuwe client die verbonden is.
 */
int TCPServer::acceptClient() {
    sockaddr_in addr{};
    socklen_t len = sizeof(addr);

    int newClient = accept(server_fd, (sockaddr*)&addr, &len);
    if (newClient >= 0) {
        client_fd = newClient;
    }

    return newClient;
}

/**
 * @brief Returned de clientFD.
 * 
 * @return int de client_fd.
 */
int TCPServer::getClientFd() const {
    return client_fd;
}

/**
 * @brief Returned de serverFD.
 * 
 * @return int de server_fd.
 */
int TCPServer::getServerFd() const {
    return server_fd;
}
/**
 * @brief Stuurt een bericht naar alle cliënten, hiervoor wordt de socketfunctie send() gebruikt.
 * 
 * @param msg Het bericht dat verstuurd moet worden.
 * @return int Een error waarde, wordt niks mee gedaan.
 */
int TCPServer::sendClient(const std::string& msg) {
    if (client_fd < 0) {
        return -1;
    }

    return send(client_fd, msg.c_str(), msg.size(), 0);
}

/**
 * @brief Verwijderd het TCPServer::TCPServer object.
 * 
 */
TCPServer::~TCPServer() {
    if (client_fd >= 0) {
        close(client_fd);
    }

    if (server_fd >= 0) {
        close(server_fd);
    }
}

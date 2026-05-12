#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <string>

/**
 * @brief TCPServer class. Listens for connections and accepts client
 * that want to connect. 
 * 
 */
class TCPServer{
public:
    TCPServer();

    bool start(int port);
    int acceptClient();
    int getCLientFd() const;
    int getServerFd() const;
    int readClient(char* buffer, int size);
    int sendClient(const std::string& msg);

    ~TCPServer();

private:
    int server_fd = -1;
    int client_fd = -1;
};

#endif

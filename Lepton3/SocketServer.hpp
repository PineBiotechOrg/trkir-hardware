#ifndef TRKIR_HARDWARE_SOCKET_SERVER_HPP
#define TRKIR_HARDWARE_SOCKET_SERVER_HPP

#include <boost/thread.hpp>
#include <boost/algorithm/string/join.hpp>
#include <iostream>
#include <cstring>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


class SocketServer {
public:
    SocketServer(int port);

    ~SocketServer();

    void accept();

    void send(uint8_t *message, int length);

private:
    int _socket_fd;
    int _client_socket_fd;
};


#endif //TRKIR_HARDWARE_SOCKET_SERVER_HPP

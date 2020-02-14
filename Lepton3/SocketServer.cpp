#include "SocketServer.hpp"


SocketServer::SocketServer(int port) {
    struct sockaddr_in server_addr;
    int ret;

    std::memset(&server_addr, 0, sizeof(server_addr));

    _socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket_fd < 0) {
        std::cout << "Can't open socket\n";
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    ret = bind(_socket_fd, reinterpret_cast<struct sockaddr *>(&server_addr), sizeof(server_addr));
    if (ret < 0) {
        std::cout << "Can't bind to port\n";
    }

    listen(_socket_fd, 1);
}

void SocketServer::accept() {
    std::cout << "Waiting for connection...\n";
    struct sockaddr_in client_addr;
    socklen_t client_addr_length = sizeof(client_addr);

    _client_socket_fd = ::accept(_socket_fd, reinterpret_cast<struct sockaddr *>(&client_addr), &client_addr_length);
    std::cout << "Oops someone connected...\n";
}

void SocketServer::send(uint8_t *message, int length) {
    if (_client_socket_fd) {
        std::string transformed = std::to_string(message[0]);

        // TODO: переделать на векторы https://tracker.yandex.ru/VPAGROUPDEV-906
        for (int s = 1; s < length; ++s) {
            transformed += ("," + std::to_string(message[s]));
        }
        transformed += "\r\n";

        boost::thread send_thread{::send, _client_socket_fd, transformed.c_str(), transformed.length(), 0};
        send_thread.join();
    }
}

SocketServer::~SocketServer() {
    if (_client_socket_fd) {
        close(_client_socket_fd);
    }
    if (_socket_fd) {
        close(_socket_fd);
    }
}

#include <iostream>
#include <signal.h>

#include "Lepton.hpp"
#include "SocketServer.hpp"

bool is_keep_streaming = true;

void stop_streaming(int signal) {
    is_keep_streaming = false;
}


int main(int argc, char const *argv[]) {
    signal(SIGINT, stop_streaming);

    auto *details = new SPIConnectionDetails;
    int socket_port = 8080;

    details->device = SPI_DEVICE;
    details->mode = SPI_MODE;
    details->speed = SPI_SPEED;
    details->bits = SPI_BITS;
    details->print = true;

    auto *spi_conn = new SPIConnection(details);

    if (spi_conn->is_connected()) {
        auto *copy_image = new uint8_t[FRAME_BUFFER_LENGTH];
        auto *image = new uint8_t[FRAME_BUFFER_LENGTH];

        auto *socket = new SocketServer(socket_port);
        boost::thread accept_thread{boost::bind(&SocketServer::accept, socket)};
        accept_thread.detach();

        auto *camera = new Lepton(spi_conn);

        std::cout << "Starting streaming...\n";
        while (is_keep_steaming) {

            camera->make_image();
            image = camera->get_image();


            std::copy(image, image + FRAME_BUFFER_LENGTH, copy_image);
            socket->send(copy_image, FRAME_BUFFER_LENGTH);
        }

        if (socket) {
            delete socket;
        }
        if (camera) {
            delete camera;
        }
        if (copy_image) {
            delete []copy_image;
        }
        if (image) {
            delete []image;
        }
    } else {
        std::cout << "Couldn't connect";
    }
    if (details) {
        delete details;
    }
    if (spi_conn) {
        delete spi_conn;
    }

    return 0;
}

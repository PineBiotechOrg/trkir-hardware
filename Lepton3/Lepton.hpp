
#ifndef LEPTON3_LEPTON_HPP
#define LEPTON3_LEPTON_HPP

#include <cstdlib>

#include "SPI.hpp"

// 3 frames - 4 segments - 60 packets - 164 bytes in one spi transaction
#define PACKET_BUFFER_LENGTH 164
#define FRAMES_COUNT 3
#define FRAME_BUFFER_LENGTH 4 * 60 * 164
#define WRONG_PACKET 0x0F
#define END_STATUS_BITS 0x0F
#define PACKETS_COUNT 60
#define SEGMENTS_COUNT 4
#define PACKET_WITH_SEGMENT_NUMBER 20


class Lepton {
public:
    Lepton(SPIConnection *spi_conn);

    ~Lepton();

    void make_image();

    uint8_t *get_image();

private:
    SPIConnection *_spi_conn;
    uint8_t *_rx_buf;
    uint8_t *_image;
};


#endif //LEPTON3_LEPTON_HPP

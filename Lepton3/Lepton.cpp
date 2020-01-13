#include <stdlib.h>

#include "Lepton.hpp"


int is_new_segment_start(uint8_t *rx_buf, int packet_number, int packet) {
    int segment_number;

    if ((packet_number == 0) && ((packet + PACKETS_COUNT * PACKET_BUFFER_LENGTH) < FRAMES_COUNT * FRAME_BUFFER_LENGTH)) {
        segment_number = rx_buf[packet + PACKET_WITH_SEGMENT_NUMBER * PACKET_BUFFER_LENGTH] >> 4;
        if ((segment_number > 0) && (segment_number <= SEGMENTS_COUNT) &&
            (rx_buf[packet + PACKET_WITH_SEGMENT_NUMBER * PACKET_BUFFER_LENGTH + 1] == PACKET_WITH_SEGMENT_NUMBER)) {
            return segment_number;
        }
    }

    return -1;
}

// status_bits will be 0x0F if all four segments were found
uint8_t move_status_bits(int segment_number) {
    return 0x01 << (segment_number - 1);
}


Lepton::Lepton(SPIConnection *spi_conn)
: _spi_conn(spi_conn) {
    _rx_buf = new uint8_t[FRAMES_COUNT * FRAME_BUFFER_LENGTH];
    if (!_rx_buf) {
        perror("Couldn't allocate memory for rx_buf");
    }

    _image = new uint8_t[FRAME_BUFFER_LENGTH];
    if (!_image) {
        perror("Couldn't allocate memory for image");
    }
}

void Lepton::make_image() {
    uint8_t status_bits = 0;

    int row;
    int packet;
    int packet_number, segment_number;
    int current_segment_number;
    bool valid_segment = false;

    while (status_bits != END_STATUS_BITS) {
        _spi_conn->transfer(NULL, _rx_buf, FRAMES_COUNT * FRAME_BUFFER_LENGTH);

        for (int packet_index = 0; packet_index < ((FRAMES_COUNT * FRAME_BUFFER_LENGTH) / PACKET_BUFFER_LENGTH); ++packet_index) {
            packet = packet_index * PACKET_BUFFER_LENGTH;

            if ((_rx_buf[packet] & WRONG_PACKET) == WRONG_PACKET) {
                valid_segment = false;
                continue;
            }

            packet_number = _rx_buf[packet + 1];

            if ((packet_number > 0) && !valid_segment) {
                continue;
            }

            if (valid_segment && (packet_number == 0)) {
                valid_segment = false;
            }

            if (!valid_segment) {
                segment_number = is_new_segment_start(_rx_buf, packet_number, packet);
                if (segment_number > 0) {
                    valid_segment = true;
                    current_segment_number = segment_number;
                }
            }

            if (!valid_segment) {
                continue;
            }

            row = packet_number + (segment_number - 1) * PACKETS_COUNT;
            memcpy(_image + row * PACKET_BUFFER_LENGTH, _rx_buf + packet, PACKET_BUFFER_LENGTH);

            if (packet_number == PACKETS_COUNT - 1) {
                // this way status bits will be 0x0F only if all 4 segments were found
                status_bits |= move_status_bits(current_segment_number);
            }
        }
    }
    return;
}

uint8_t *Lepton::get_image() {
    return _image;
}

Lepton::~Lepton() {
    if (_rx_buf) {
        delete []_rx_buf;
    }
    if (_image) {
        delete []_image;
    }

    printf("\nLepton destructed\n");
}

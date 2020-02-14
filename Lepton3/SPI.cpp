#include <stdlib.h>

#include "SPI.hpp"


SPIConnection::SPIConnection(SPIConnectionDetails *details)
: _details(details) {
    int ret;

    _is_connected = false;

    // open device
    _file_descriptor = open(_details->device.c_str(), O_RDWR);
    if (_file_descriptor < 0) {
        // TODO: логирование https://tracker.yandex.ru/VPAGROUPDEV-907
        std::cout << "Error open device";
        return;
    }

    // set mode
    ret = ioctl(_file_descriptor, SPI_IOC_WR_MODE, &_details->mode);
    if (ret < 0) {
        std::cout << "Error set SPI-Mode";
    }

    // check mode
    ret = ioctl(_file_descriptor, SPI_IOC_RD_MODE, &_details->mode);
    if (ret < 0) {
        std::cout << "Error get SPI-Mode";
    }

    // set word length
    ret = ioctl(_file_descriptor, SPI_IOC_WR_BITS_PER_WORD, &_details->bits);
    if (ret < 0) {
        std::cout << "Error set word length";
        return;
    }

    // check word length
    ret = ioctl(_file_descriptor, SPI_IOC_RD_BITS_PER_WORD, &_details->bits);
    if (ret < 0) {
        std::cout << "Error get word length";
        return;
    }

    // set speed
    ret = ioctl(_file_descriptor, SPI_IOC_WR_MAX_SPEED_HZ, &_details->speed);
    if (ret < 0) {
        std::cout << "Error set speed";
        return;
    }

    // check speed
    ret = ioctl(_file_descriptor, SPI_IOC_RD_MAX_SPEED_HZ, &_details->speed);
    if (ret < 0) {
        std::cout << "Error get speed";
        return;
    }

    _is_connected = true;

    if (_details->print) {
        print_info();
    }
}

void SPIConnection::print_info() {
    printf("\n\nSPI-Device.....: %s\n", _details->device.c_str());
    printf("SPI-Mode.......: %d\n", _details->mode);
    printf("Bits per word.....: %d\n", _details->bits);
    printf("Speed..........: %d Hz (%d kHz)\n", _details->speed, _details->speed / 1000);
    printf("File descriptor.....: %d\n\n\n", _file_descriptor);
}

bool SPIConnection::is_connected() {
    return _is_connected;
}

void SPIConnection::transfer(uint8_t *tx_buf, uint8_t *rx_buf, size_t length) {
    int ret;

    // clear memory
    std::memset(&_spi, 0, sizeof(_spi));

    // set spi_ioc_transfer parameters
    _spi.tx_buf = static_cast<unsigned long>tx_buf;
    _spi.rx_buf = static_cast<unsigned long>rx_buf;
    _spi.len = length;
    _spi.delay_usecs = static_cast<uint16_t>SPI_DELAY;  // test different delays and see what works for you
    _spi.speed_hz = _details->speed;
    _spi.bits_per_word = _details->bits;
    _spi.cs_change = 0;

    ret = ioctl(_file_descriptor, SPI_IOC_MESSAGE(1), &_spi);

    if (ret < 1) {
        std::cout << "Can't send / read spi data";
    }
}

SPIConnection::~SPIConnection() {
    if (_file_descriptor) {
        close(_file_descriptor);
    };
    printf("\nSPI destructed\n");
}

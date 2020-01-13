#ifndef LEPTON3_SPI_HPP
#define LEPTON3_SPI_HPP

#include <fcntl.h>     // needed for SPI port
#include <sys/ioctl.h> // needed for SPI port
#include <errno.h>     // linux errors
#include <unistd.h>    // usleep()
#include <stdio.h>
#include <sys/time.h>
#include <stdint.h>    // uint8_t,  uint16_t etc
#include <linux/spi/spidev.h> //  spi_ioc_transfer
#include <cstring>
#include <string>


#define SPI_DEVICE "/dev/spidev0.0"
// SPI_CPOL | SPI_CPHA = 3
#define SPI_MODE 3
#define SPI_SPEED 18000000
#define SPI_BITS 8
// can be changed to lower if works
#define SPI_DELAY 65000

struct SPIConnectionDetails {
    std::string device;
    int mode;
    int speed;
    int bits;
    bool print;
};

class SPIConnection {
public:
    SPIConnection(SPIConnectionDetails *details);

    ~SPIConnection();

    bool is_connected();

    void transfer(uint8_t *tx_buf, uint8_t *rx_buf, size_t length);

    void print_info();

private:
    SPIConnectionDetails *_details;

    int _file_descriptor;
    bool _is_connected;
    struct spi_ioc_transfer _spi;

};


#endif //LEPTON3_SPI_HPP

#include <iostream>
#include <boost/thread.hpp>

#include "Lepton.hpp"



// saves frame buffer to txt file
void save_image(uint8_t *copy_image, int index) {
    char file_name[124];
    snprintf(file_name, sizeof(file_name), "%d.txt", index);

    FILE *f = fopen(file_name, "w");

    for (int i = 0; i < FRAME_BUFFER_LENGTH; i++) {
        fprintf(f, "%d ", copy_image[i]);
    }

    fclose(f);
}

int main(int argc, char const *argv[]) {
    auto *details = new SPIConnectionDetails;

    details->device = SPI_DEVICE;
    details->mode = SPI_MODE;
    details->speed = SPI_SPEED;
    details->bits = SPI_BITS;
    details->print = true;

    auto *spi_conn = new SPIConnection(details);

    if (spi_conn->is_connected()) {
        auto *copy_image = new uint8_t[FRAME_BUFFER_LENGTH];
        auto *image = new uint8_t[FRAME_BUFFER_LENGTH];

        auto *camera = new Lepton(spi_conn);

        for (int s = 0; s < 100; ++s) {
            camera->make_image();
            camera->get_image();

//            memcpy(copy_image, image, FRAME_BUFFER_LENGTH * sizeof(uint8_t));
//            boost::thread thread{save_image, copy_image, s};
//            thread.join();

            printf("next...\n");
        }

        delete []copy_image;
        delete []image;

        return 0;
    } else {
        perror("Couldn't connect");
        return -1;
    }
}


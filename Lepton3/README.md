### Required packages ###
```sudo apt-get install libboost-all-dev```

### Build (CMake soon)

```g++ -o spi -pthread -lboost_system -lboost_thread main.cpp SPI.cpp SPI.hpp Lepton.cpp Lepton.hpp```

### Increasing the SPI Buffer Size
Increase the spidev buffer size by adding "spidev.bufsiz=131072" to /boot/cmdline.txt
``` 
sudo /boot/cmdline.txt

Add "spidev.bufsiz=131072" to the arguments

ctrl+o to save
ctrl+x to exit

sudo reboot
```

### Camera connection ###
https://lepton.flir.com/forums/topic/pylepton-overlay-guide-2-0-for-raspbian-pixel/

### FLIR Lepton engineering datasheet ###
./resources/flir-lepton-engineering-datasheet.pdf
OR
https://www.flir.com/globalassets/imported-assets/document/flir-lepton-engineering-datasheet.pdf?_ga=2.20065144.1943332102.1578738122-1344865710.1578391315

### More on Lepton ###
./resources/overview.pdf
OR
https://www.vutbr.cz/www_base/zav_prace_soubor_verejne.php?file_id=181847

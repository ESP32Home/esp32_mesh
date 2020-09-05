# esp32_mesh
mesh examples for parents and leaf with ESP32 and ESP32-S2 using the esp_idf

## projects
* parent : idf project for mesh nodes with permanent power supply
    * includes web server fo mesh view and configuration
* leaf : idf project for low power sleepy devices
    * includes sensors libraries

## Usage
for first time init see [hello example readme](https://github.com/ESP32Home/hello-esp32s2/blob/master/README.md#first-time)

in terminal
```console
> . .profile
> get_idf-4.1
> idf --version
> cd hello-esp32s2
> idf set-target esp32s2
> idf menuconfig
> idf build
> flash && monitor
```

# References
* [docs.espressif esp32 api-guide mesh](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/mesh.html)


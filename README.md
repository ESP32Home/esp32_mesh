# esp32_mesh
mesh examples for parents and leaf with ESP32 and ESP32-S2 using the esp_idf

# features
* wrap static code into a c++ app library
    * quicker portability by calling one function instead of copy pasting hundreds of lines each time
    * template code for wifi and mesh is quite stable and does not require changes on different samples
    * arduino like, makes porting later easier
* switch from compile time menuconfig to runtime json config
    * allows a structured config in json
    * update of the file anytime even remote with fs management
    * easier to interface with webapps as tool for config
* Use SmartConfig for wifi provisioning
    * possible to provide a ready to flash app without config dependencies for wifi password
    * SmartConfig is a standard provisioning method

# projects
* parent : idf project for mesh nodes with permanent power supply
    * includes web server fo mesh view and configuration
* leaf : idf project for low power sleepy devices
    * includes sensors libraries

# Mesh
* Tree topology up to 25 layers
* Chain topology up to 1000 layers

# config
* `mesh.topology` : "tree", "chain"
* `mesh.leaf.duty_type` : "request", "demand"
    * `mesh.leaf.demand_cycle` only applicable with "demand"
* `mesh.parent.network.rule`: "entire", "uplink"

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


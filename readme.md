![MCHP](https://www.microchip.com/ResourcePackages/Microchip/assets/dist/images/logo.png)

# MTCH2120
The MTCH2120 is a twelve-channel capacitive touch controller with an I2C interface, designed for applications requiring water-tolerant touch buttons or proximity sensing. It features 12 capacitive touch sensors with configurable Driven Shield capability, adjustable touch performance parameters via the I2C interface, and Easy Tune for out-of-the-box usage without the need for tuning. The device supports various conductive materials as sensors and includes advanced noise suppression algorithms, automatic environmental compensation, and low power consumption options. It operates within a voltage range of 3 to 5V and a temperature range of -40°C to +105°C

Refer to [datasheet](https://ww1.microchip.com/downloads/aemDocuments/documents/MCU08/ProductDocuments/DataSheets/MTCH2120-12CH-CapacitiveTouchControl-DataSheet-DS40002613.pdf) for more information on MTCH2120.

## Directory Structure
- [mtch2120_evk](./mtch2120_evk)
    - [bin](./mtch2120_evk/bin/)
        - [mtch2120_evk_host_1.1.0.hex](./mtch2120_evk/bin/mtch2120_evk_host_1.1.0.hex)
        - [mtch2120_evk_host_1.0.0.hex](./mtch2120_evk/bin/mtch2120_evk_host_1.0.0.hex)
        - [readme_hex_files.md](./mtch2120_evk/bin/readme_hex_files.md)
    - [firmware](./mtch2120_evk/firmware/)
        - [mtch2120_evk_host_firmware.X](./mtch2120_evk/firmware/mtch2120_evk_host_firmware.X/)
        - [src](./mtch2120_evk/firmware/src/)
        - [readme_host_firmware.md](./mtch2120_evk/firmware/readme_host_firmware.md)
    - [readme_mtch2120_evk.md](./mtch2120_evk/readme_mtch2120_evk.md)

## Useful Links
- [MTCH2120 website](https://www.microchip.com/en-us/product/MTCH2120)
- [MPLAB Harmony Touch Host Driver](https://github.com/Microchip-MPLAB-Harmony/touch_host_driver)
- [MPLAB Harmony Touch Host Application](https://github.com/Microchip-MPLAB-Harmony/touch_host_driver_apps/tree/v1.0.0/apps/MTCH2120)

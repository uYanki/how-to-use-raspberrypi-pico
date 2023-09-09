1. download lastest firmware：uf2 [MpyPico](https://micropython.org/download/RPI_PICO/ ) [MpyPicoW](https://micropython.org/download/rp2-pico-w/) [CpyPico](https://circuitpython.org/board/raspberry_pi_pico/) [CpyPicoW](https://circuitpython.org/board/raspberry_pi_pico_w/) 

2. hold down the BOOTSEL button while plugging the board into USB

3. copy uf2 to the usb storage device

   ![disk](Images/disk.png)

---

### Flashing via UF2 bootloader

To get the board in bootloader mode ready for the firmware update, execute `machine.bootloader()` at the MicroPython REPL. Alternatively, hold down the BOOTSEL button while plugging the board into USB. The uf2 file below should then be copied to the USB mass storage device that appears. Once programming of the new firmware is complete the device will automatically reset and be ready for use.
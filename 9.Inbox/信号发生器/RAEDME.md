开源一个基于RP2040的便携式任意波形发生器，用RP2040的PIO直驱并口DAC。改进自rgco的版本（[color=rgb(85, 136, 255) !important][Arbitrary Wave Generator With the Raspberry Pi Pico](https://www.instructables.com/Arbitrary-Wave-Generator-With-the-Raspberry-Pi-Pic/)），R-2R网络改为DAC904，并添加了抗镜像滤波器和输出运放。 RP2040超频到250M时可实现125MSa/S的采样率。 **核心指标** 采样率：125MSa/S位数：14bit输出幅值范围：0\~2Vpp输出频率范围：0~50MHz（正弦）  

PCB：https://oshwhub.com/32478yf4780gf72r49fg4/picoawg  

固件：https://github.com/leidawt/PicoAWG-Firmware 

上位机：https://github.com/leidawt/PicoAWG-Software 
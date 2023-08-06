https://github.com/dotcypress/ula


不同于之前给大家分享的树莓派单片机逻辑分析仪方案，这款分析仪直接使用sigrok现成的上位机方案PulseView。sigrok功能很强大，各种仪器设备开发都可以使用。


树莓派单片机的16通道100MHz逻辑分析仪  μLA Micro Logic Analyzer
作者 牛人一枚 。 源码是  rust 写的 ！可作为 学习 rust的 模板！

构建固件
按照 https://rustup.rs 中的说明安装 rustup
安装 Cortex-M0、M0+ 和 M1（ARMv6-M 架构）目标：rustup target add thumbv6m-none-eabi
安装 LLVM 工具：rustup component add llvm-tools-preview
安装 cargo-binutils：（注意：在某些 Linux 发行版（例如 Ubuntu）上，
您可能需要在安装 cargo-binutils 之前安装 build-essential、gcc-arm-none-eabi、libssl-dev 和 pkg-config 软件包）cargo install cargo-binutils
安装 elf2uf2：cargo install elf2uf2-rs
克隆此存储库：git clone git@github.com:dotcypress/ula.git && cd ula
按住 BOOTSEL 按钮，同时将主板连接到计算机
Flash microcontroller:：cargo run --release
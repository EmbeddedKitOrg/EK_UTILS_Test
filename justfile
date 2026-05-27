alias c := clean
alias b := build
alias f := flash
alias m := monitor
alias r := run

clean:
    rm -rf build

build:
    cmake -DCMAKE_TOOLCHAIN_FILE=cmake/gcc-arm-none-eabi.cmake \
    -DPLATFORM=cm4  \
    -DPICOLIBC=1 \
    -B build -G Ninja 
    ninja -C build

flash:build
    probe-rs download --speed 4000 --chip-description-path GD32F4xx_Series.yaml --chip GD32F470ZG ./build/ek_utils_test.elf
    probe-rs reset --chip-description-path GD32F4xx_Series.yaml --chip GD32F470ZG
        
monitor:
    probe-rs reset --chip-description-path GD32F4xx_Series.yaml --chip GD32F470ZG
    tio -b 1500000 /dev/ttyACM0 

run:build flash monitor

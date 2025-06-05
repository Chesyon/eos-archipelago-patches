# Linux setup guide

You can install c-of-time on Linux using the following methods. Instructions vary based on your Linux distribution.

- [Ubuntu/Debian](#ubuntu-debian)
- [Fedora](#fedora)
- [Arch Linux](#arch-linux)
- [Other methods (advanced)](#other-methods-advanced)

## Ubuntu/Debian

You can install eos-archipelago-patches on Ubuntu or Debian using the following methods. The steps detailed in this guide were tested on Ubuntu 22.04.

1. Open the Terminal app in your Applications menu. The exact steps might vary based on your desktop environment.
2. Run the following command to install the required tools: `sudo apt install build-essential cmake python3-pip gcc-arm-none-eabi binutils-arm-none-eabi xdelta3`. You will need to enter your password during the installation.
3. Install Python dependencies: `pip install pyyaml ndspy git+https://github.com/Eijebong/bsdiff4f@baba7e24ca5f301b633b4d4de3e1788308d6ac06`
    - **Note:** If this does not work, try `pip install pyyaml ndspy git+https://github.com/Eijebong/bsdiff4f@baba7e24ca5f301b633b4d4de3e1788308d6ac06 --break-system-packages`.
4. Compile `armips`:
    1. Run `git clone --recursive https://github.com/Kingcom/armips.git` to download the source code.
    2. Run `cd armips` to enter the directory.
    3. Run this command to compile the project: `mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build .`
    4. Run `sudo cp armips /usr/local/bin` to install `armips`.
    5. Run `cd ../..` to return to the previous directory.
5. Navigate to the directory where you want to install eos-archipelago-patches. You can use `cd` to change the directory and `ls` to list the contents of the current directory. For example, if you want to install eos-archipelago-patches to `/home/YourName/Documents/eos-archipelago-patches`, run `cd /home/YourName/Documents`.
    - **Note:** You can also use the file manager to navigate to the directory where you want to install eos-archipelago-patches. Right-click the name of the directory and select "Open in Terminal".
6. Download this repository by running `git clone --recursive https://github.com/Chesyon/eos-archipelago-patches.git` in the terminal. eos-archipelago-patches will be downloaded in a folder called `eos-archipelago-patches` inside the current directory.
7. Enter the `eos-archipelago-patches` directory with `cd eos-archipelago-patches`.
8. Copy your vanilla ROM you have prepared into the `eos-archipelago-patches` directory and rename it to `vanilla.nds`. You can open the file manager in the current directory by running `xdg-open .` in the terminal.

## Fedora

1. Open the Terminal app in your Applications menu. The exact steps might vary based on your desktop environment.
2. Run the following command to install the required tools: `sudo dnf install @development-tools gcc-c++ cmake python3-pip arm-none-eabi-binutils-cs arm-none-eabi-gcc-cs xdelta3`. You will need to enter your password during the installation.
3. You can now continue with the steps 3-10 of the Ubuntu/Debian method above.

## Arch Linux

1. Open the Terminal app. The exact steps vary based on your desktop environment.
2. Run the following command to install the required tools: `sudo pacman -Syu base-devel git python python-pip arm-none-eabi-gcc arm-none-eabi-binutils xdelta3`. You will need to enter your password during the installation.
3. Install Python dependencies: `pip3 install pyyaml ndspy bsdiff`
4. Install the [armips package](https://aur.archlinux.org/packages/armips) via the Arch User Repository (AUR). Please refer to the [Arch Wiki](https://wiki.archlinux.org/title/Arch_User_Repository) for instructions.
5. You can now continue with the steps 5-10 of the Ubuntu/Debian method above.

## Other methods (advanced)

- You can use the official [ARM toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads) to build c-of-time. Make sure to add the toolchain to your `PATH` environment variable. You will need to install `make` and other Unix build tools as well.
- [devkitpro](https://devkitpro.org/wiki/Getting_Started) provides an installer for a custom toolchain. Follow the instructions on their website to install it and make sure to add the toolchain to your `PATH` environment variable.

# Windows setup guide

You can install eos-archipelago-patches on Windows using the following methods. The steps detailed in this guide were tested with Windows 11. This guide assumes that you have a 64-bit version of Windows installed.

- [Method 1: WSL (recommended)](#method-1-wsl-recommended)
- [Method 2: MSYS2](#method-2-msys2)
- [Other methods (advanced)](#other-methods-advanced)

## Method 2: WSL (recommended)

You can run eos-archipelago-patches in the Windows Subsystem for Linux (WSL). WSL 2 is recommended for maximum compatibility.

Refer to [this guide by Microsoft](https://learn.microsoft.com/en-us/windows/wsl/install) to install WSL. After the installation is finished, open a terminal such as the Windows Command Prompt or Windows PowerShell and run `wsl.exe` to enter WSL. You can now install follow the [setup instructions for Linux](./install_linux.md).

## Method 2: MSYS2

MSYS2 is a Unix-like environment for Windows. It provides a terminal and a package manager that allows you to install the required tools on Windows with a few commands. This is the recommended method for installing eos-archipelago-patches on Windows.

Note: There are known issues with package installation using this method (see [this thread in the SkyTemple Discord server](https://discord.com/channels/710190644152369162/1320206615475130399/1320206615475130399)).

1. Download and install [MSYS2](https://www.msys2.org/wiki/MSYS2-installation/) by following the instructions on their website. This guide assumes that you installed MSYS2 to `C:\msys64`, adjust the paths accordingly if you installed it to a different location.
2. Press the Windows key, search for "MINGW64" and open the "MSYS2 MINGW64" shortcut to open a terminal. **Note that the other MSYS shortcuts will not work, always open the "MSYS2 MINGW64" one.**
3. Run `pacman -S git make mingw-w64-x86_64-arm-none-eabi-toolchain mingw-w64-x86_64-python-pip xdelta3` to install Git, Make, the ARM toolchain and the Python package manager. Confirm that you want to install all packages by pressing enter, then type `y` and press Enter.
4. Install Python dependencies: `pip3 install pyyaml ndspy git+https://github.com/Eijebong/bsdiff4f@baba7e24ca5f301b633b4d4de3e1788308d6ac06`
5. Navigate to the directory where you want to install eos-archipelago-patches. You can use `cd` to change the directory and `ls` to list the contents of the current directory. For example, if you want to install eos-archipelago-patches to `C:\Users\YourName\Documents\eos-archipelago-patches`, run `cd /c/Users/YourName/Documents`.
    - **Note:** You can also use the Windows Explorer to navigate to the directory where you want to install eos-archipelago-patches. Right-click in the directory and select "Open in Terminal", then run `C:\msys64\msys2_shell.cmd -mingw64 -here` to open a terminal in that directory.
6. Download this repository by running `git clone --recursive https://github.com/Chesyon/eos-archipelago-patches.git` in the MSYS2 terminal. eos-archipelago-patches will be downloaded in a folder called `eos-archipelago-patches` inside the current directory.
7. Enter the `eos-archipelago-patches` directory with `cd eos-archipelago-patches`.
8. Copy the ROM you have prepared into the `eos-archipelago-patches` directory and rename it to `vanilla.nds`. You can open Windows Explorer in the current directory by running `explorer .` in the MSYS2 terminal.
9. Run `make headers` to add aliases and documentation comments to headers for increased compatibility.
10. Run `make out` to build the project. The output ROM will be saved as `out.nds`.
    - If you are encountering errors with armips, you might need to install the [Visual C++ Redistributable for Visual Studio 2015](https://www.microsoft.com/en-US/download/details.aspx?id=48145). Make sure to download the 64-bit version (`vc_redist.x64.exe`).

## Other methods (advanced)

- You can use the official [ARM toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads) to build eos-archipelago-patches. Make sure to add the toolchain to your `PATH` environment variable. You will need to install `make` and other Unix build tools as well.
- [devkitpro](https://devkitpro.org/wiki/Getting_Started) provides a Windows installer for a custom toolchain. Follow the instructions on their website to install it and make sure to add the toolchain to your `PATH` environment variable.

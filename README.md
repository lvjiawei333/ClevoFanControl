[简体中文](README_zh_cn.md)

# This English version of this article is still under construction

# ClevoFanControl

Clevo laptop fan control utility

**Modifying Hardware is risky, the Author is not responsible for any damage caused by this software!**

If you meet bugs while using this software, feel free to open an [Issue](https://github.com/elight2/ClevoFanControl/issues)

## Usage

**For detailed instructions on how to use, see [Feature Introduction](#Feature-Introduction)**

### Windows

1. Download the [latest release](https://github.com/elight2/ClevoFanControl/releases/latest) for Windows and extract the zip file
2. Run `ClevoFanControl-gui` as Administrator

### Linux

1. Download the [latest release](https://github.com/elight2/ClevoFanControl/releases/latest) for Windows and extract the tar file
2. Make sure that the lib files of Qt6 are installed on your os, or instead, you can set environment variables to make the program load your Qt6 `.so` files
3. Run `cfc-launcher.sh` as root

#### Note (Must Read)

* Directly running `ClevoFanControl-gui` as root in linux may cause the program to run in the background without displaying the system tray. Using `cfc-launcher.sh`, which executes `unset XDG_CURRENT_DESKTOP` before launching the program, resolves this issue
* Remember to switch to `game***` in the system tray when your PC is under high load to prevent the motherboard from overheating
* Turn on the `Monitor GPU` switch in the system tray when your dGPU is active or in use, or the program will not monitor GPU temperature. See [Explanation of GPU Temperature Monitoring](#Explanation-of-GPU-Temperature-Monitoring) for detailed info
* The default profiles are made in model m960kp according to the author's preferences and habits, so they may not apply to other users/models, it's strongly recommended to read [Feature Introduction](#Feature-Introduction) and modify the profiles by yourself

## Feature Introduction

### System Tray

* Monitor: display the temperature and power of CPU and GPU, and the fan speed signals and the actual RPM of the 2 fans
* Config: management program settings
* Profile: select the profile you'd like to use
* Commands: execute your customized commands
* Max Speed: set both fan speed to 100%
* Speed Limit: enable speed limit feature (can be configured in Config page)
* Static Speed: enable static speed mode (can be configured in Config page, mainly used for hardware debug)
* Clevo Auto: use the default fan speed adjustment method in Clevo laptops
* Monitor GPU: enable GPU monitoring
* Exit: stop the program, and set the fans to "Clevo Auto"

### Configuration

* Currently, the multi-profile function only supports selecting and editing existing ones in GUI. If you want to add/remove/rename a profile, you have to edit `config.json` manully.
* Currently, the function of custom commands only supports executing commands saved in `config.json`, to add/remove/rename a command, please edit it manully. The 2 original command entries each serves the functions of limiting the GPU frequency to 1200Mhz and reseting the frequency limitation respectively.
* The default `normal` profile is suitable for daily work while the `game4030` one is made for gaming scene with higher min fan speed

#### Configuration File (`config.json`)

* Any newer version of this software **does not support** automatically upgrading config file from older versions, after upgrading, you have to edit

### Explanation of GPU Temperature Monitoring

### mechanism
* `ec.zip` in this repository contains all the referance resource I found.
#### Fan control interface
* Windows: [WinRing0](https://github.com/GermanAizek/WinRing0) [pre-compiled binaries](https://github.com/QCute/WinRing0)
* Linux: `inb()` and `outb()`
#### Temperature query interface
* Windows: CPU: MSR register `IA32_PACKAGE_THERM_STATUS_MSR`, GPU: `nvidia-smi -q -d=TEMPERATURE`
* Linux: CPU: `/sys/class/thermal/thermal_zone*/temp`, GPU: `nvidia-smi -q -d=TEMPERATURE`
*It seems that nvidia driver version must be higher than 5xx.xx to correctly read GPU temp*

### Build
* Tools: cmake, ninja, gcc, qt6
1. clone the repository
2. Modify the build scrips in `src/scripts/`, make sure the cmake configure options are correct, finally run it

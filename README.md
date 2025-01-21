[简体中文](README_zh_cn.md)

# ClevoFanControl

Clevo laptop fan control utility

**Modifying Hardware is risky, the Author is not responsible for any damage that may occur. Please read the feature description before proceeding!**

If you encounter bugs dring use, feel free to open an [Issue](https://github.com/elight2/ClevoFanControl/issues)

## Usage

**For detailed information, see [Feature Introduction](#Feature-Introduction)**

### Windows

1. Download and extract the [latest release](https://github.com/elight2/ClevoFanControl/releases/latest) for Windows
2. Run `ClevoFanControl-gui` as Administrator

### Linux

1. Download and extract the [latest release](https://github.com/elight2/ClevoFanControl/releases/latest) for Linux
2. Ensure Qt6 libraries are installed, or instead, you can set environment variables to make the program locate your Qt6 `.so` files
3. Run `cfc-launcher.sh` as root

#### Note (Must Read)

* Directly running `ClevoFanControl-gui` as root on linux may result in a missing tray icon. Using `cfc-launcher.sh`, which executes `unset XDG_CURRENT_DESKTOP` before launching the program, resolves this issue
* For high loads, switch to `game***` profile in the system tray to prevent overheating
* Turn on the `Monitor GPU` switch in the system tray when your dGPU is active or in use, otherwise GPU temperature will not be monitored. See [Explanation of GPU Temperature Monitoring](#Explanation-of-GPU-Temperature-Monitoring) for detailed info
* The default profiles are made for m960kp (Hasee TX8) based on the author's usage habits, so they may be not suitable for other users/models. Please read [Feature Introduction](#Feature-Introduction) and modify the profiles accordingly

## Feature Introduction

### System Tray

* Monitor: Display the temperature, power, fan speed signal and actual fan speed of CPU/GPU
* Config: Manage software settings
* Profile: Select the profile to use
* Commands: Execute customized commands
* Max Speed: Set both fan speed to 100%
* Speed Limit: Activate speed limit feature (can be configured in Config page)
* Static Speed: Enable static speed mode (can be configured in Config page, mainly used for hardware debug)
* Clevo Auto: Use the default fan speed adjustment method in Clevo laptops
* Monitor GPU: enable GPU monitoring
* Exit: Exit the program, and set the fans to "Clevo Auto"

### Configuration

* Currently, the multi-profile function only supports selection and editing existing profiles in GUI. If you want to add/remove/rename a profile, you have to edit `config.json` manully.
* Currently, the function of custom commands only supports executing commands saved in `config.json`, to add/remove/rename/edit a command, please edit the configuration file. The two original command entries each serves the functions of limiting the GPU frequency to 1200Mhz and reseting the frequency limitation respectively.
* The default `normal` profile is for daily use while the `game4030` one is made for gaming scene with higher min fan speed

#### Configuration File (`config.json`)

* Any newer version of this software **does not support** automatically upgrading config file from older versions, after upgrading, you have to migrate the old config file manually: run the new version once to let it generate a new version of `config.json`, then copy the corresponding entries from the old file into the new one
* Configuration file is saved whenever tray settings are changed to avoid data loss during shutdown

#### Config GUI

*Manual JSON file editing is recommended instead of using GUI*

* Profile: Select the profile to edit. (to select which profile to use, use system tray)
* Temp-Speed mode: If the core temperature is between 2 temp values, set the fan spped to the corresponding speed value of the lower temp
* Max Temp mode: The fan speed will be improved by "Speed Step" if the core temperature is higher than "Speed Up Temp", and will be lowered by "Speed Step" if it's lower than "Slow Down Temp". The speed won't be lower than "Min Speed". **Note: this is the recommended mode to use, which produces lower fan noise, but slightly affect performance. As it has low fan speed, the motherboard heats up easily, so it's strongly recommended to set the "Min Speed" a bit higher to cool it down, especially when gaming**
* Static Speed: Set the fan speed percentage to specific values, mainly used for debugging hardware. Check "enabled" to turn it on
* Speed Limit: Limit the max speed of the fans. Fan speeds of both mode won't be higher than the limitation when enabled
* Fan Intervals: Time intervals (in ms) for check core temperature and adjusting fan speed. Low values (e.g. <=100ms) may cause EC issue
* Clevo Auto: See [System Tray](#System-Tray)
* Max Speed: See [System Tray](#System-Tray)
* Apply button: Apply the configuration and save the config file
* OK button: Apply the configuration, save the config file and close the window

### Commands Feature

* Its main purpose is to execute hardware adjusting commands as root/Administrator conveniently
* The `GPU-1200` command limits nvidia GPU clock to 1200Mhz, while the `GPU-rgc` restores the default clock limitation

### Explanation of GPU Temperature Monitoring

* Currently, `nvidia-smi` is used for GPU temperature detection. However, running `nvidia-smi` activates the GPU, causing additional power consumption, resulting in high temperature and energy waste when the dGPU is not in use (normally, nvidia GPU go to "sleep" state when idle). As determining whether the nvidia GPU is in use is hard, the temperature monitoring have to be enabled manually when you use dGPU. The Experimental GPU usage auto detection functionality is explained as follow
* GUI configuration is not supported
* Windows is not supported, but theoretically possible. Suggestions are welcome via [issue](https://github.com/elight2/ClevoFanControl/issues)
* Enabling "Monitor GPU" in tray forces GPU monitoring regardless of the auto detection settings
* Auto detection is disabled when `gpuAutoDetect` is set to `false` in `config.json`
* When `gpuAutoDetect` is set to `true`, the program automatically detects GPU usage by checking specific files under `/sys` and `/dev`, deciding whether to monitor GPU's temperature and power. The detection is implemented in function `GpuFanController::getTemp` in source code.

### Command-Line Version (ClevoFanControl-cmd)

* This version is mainly designed for some linux operating systems without GUI. This version can run without Qt6 libraries.
* For detailed usage, use `--help` or `/?` argument
* **Important: Data entered in the version is sent directly to EC without safety check (e.g. max and min value). Make sure you input the correct value**

### Auto Start on Boot

#### Windows

1. Create a custom task in "Task Scheduler"
2. Under "Action", set the task to launch `ClevoFanControl-gui.exe` and set its containing directory as the start location
3. Set "Trigger" to activate on user login
4. Check "Run with highest privilege"

#### Linux

### mechanism
* `ec.zip` in this repository contains all the referance resource I found.
#### Fan control interface
* Windows: [WinRing0](https://github.com/GermanAizek/WinRing0) [pre-compiled binaries](https://github.com/QCute/WinRing0)
* Linux: `inb()` and `outb()`
#### Temperature query interface
* Windows: CPU: MSR register `IA32_PACKAGE_THERM_STATUS_MSR`, GPU: `nvidia-smi -q -d=TEMPERATURE`
* Linux: CPU: `/sys/class/thermal/thermal_zone*/temp`, GPU: `nvidia-smi -q -d=TEMPERATURE`

### Build
* Tools: cmake, ninja, gcc, qt6
1. clone the repository
2. Modify the build scrips in `src/scripts/`, make sure the cmake configure options are correct, finally run it

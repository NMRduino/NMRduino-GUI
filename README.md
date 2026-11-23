# NMRduino GUI

NMRduino GUI is the desktop software for the [NMRduino spectrometer](https://github.com/NMRduino/NMRduino). It includes three tools for running experiments: Spectrum Analyzer, Pulse and Acquire, and Task Scheduler.

## Downloads

You can download NMRduino GUI **3.37.4** using either of the links below. The Windows package is ready to run. On a Raspberry Pi, you can build the GUI from source using the commands on this page.

| Platform | Download | Getting started |
| --- | --- | --- |
| Windows (64-bit) | [Download the compiled Windows ZIP](https://github.com/NMRduino/NMRduino-GUI/releases/download/3.37.4/nmrduino-3.37.4-win64.zip) | [How to run it on Windows](#run-on-windows) |
| Raspberry Pi, including Pi 400 | [Download the source code (.tar.gz)](https://codeload.github.com/NMRduino/NMRduino-GUI/tar.gz/refs/tags/3.37.4) | [How to build and run it on a Raspberry Pi](#build-on-raspberry-pi) |

For other versions, see [all GUI releases](https://github.com/NMRduino/NMRduino-GUI/releases). If you also need firmware for the Teensy, see [Connect the spectrometer](#connect-the-spectrometer).

## Run on Windows

1. Download the **compiled Windows ZIP** from the table above.
2. Right-click the downloaded file, choose **Extract All**, and open the extracted folder.
3. Double-click `nmrduino.exe` to start the GUI.

The ZIP includes Qt and the other files the GUI needs. If you move the program, move the whole extracted folder so these files stay together.

## Build on Raspberry Pi

These instructions are for Raspberry Pi OS with desktop, including on the Raspberry Pi 400. If you are preparing a new SD card, use [Raspberry Pi Imager](https://www.raspberrypi.com/software/) and choose a username and password under **Customisation → User**. You will use that password when `sudo` prompts for it.

**1. Install the build tools.** Open a terminal on the Pi and run the following commands. This installs the compiler, Qt 5, and the serial-port library used to communicate with the spectrometer.

```bash
sudo apt update
sudo apt install build-essential curl qt5-qmake qtbase5-dev libqt5serialport5-dev libqt5svg5
```

**2. Download the source code.** You can do this from the same terminal. The commands below download the source archive linked above and extract it into your home directory.

```bash
cd ~
curl -fL https://codeload.github.com/NMRduino/NMRduino-GUI/tar.gz/refs/tags/3.37.4 -o nmrduino-3.37.4.tar.gz
tar -xzf nmrduino-3.37.4.tar.gz
```

**3. Compile the GUI.** Run the following commands and wait for the build to finish. This step can take several minutes on a Pi.

```bash
mkdir -p ~/NMRduino-GUI-3.37.4/build
cd ~/NMRduino-GUI-3.37.4/build
/usr/lib/qt5/bin/qmake ../src/NMRduino.pro CONFIG+=release && make
```

**4. Start the GUI.** Once the terminal prompt returns after a successful build, run:

```bash
~/NMRduino-GUI-3.37.4/build/nmrduino
```

You only need to compile the GUI once. The next time you want to use it, open a terminal and run that last command.

## Connect the spectrometer

The Teensy requires NMRduino firmware **1.2.0 or later**. If you need to install or update the firmware, open the [latest NMRduino firmware release](https://github.com/NMRduino/NMRduino/releases/latest) and download the `.hex` file listed under **Assets**. Use [Teensy Loader](https://www.pjrc.com/teensy/loader.html) to flash it to the board.

Once the firmware is in place, connect the NMRduino to your computer by USB and select **NMRduino** as the acquisition device in the GUI.

You can also try the software before connecting a spectrometer. Open **Spectrum Analyzer** and select **Test DAQ** to use the simulated device.

## Settings and logs

The GUI stores settings and logs in `%USERPROFILE%\.NMRduino\` on Windows and `~/.NMRduino/` on Raspberry Pi. If you prefer to keep them with the program, place a `_config.ini` file alongside the executable to enable portable mode.

## Build and package a Windows release

If you want to build the Windows version yourself, open `src/NMRduino.pro` in Qt Creator, select a Qt 5.15.2 kit with a C++14 compiler, and build in **Release** mode. The MinGW 64-bit kit statically links the MinGW runtime. MSVC is also supported.

To create a distributable ZIP with MinGW, run the following command from the repository root in PowerShell. Adjust the two paths to match your Qt and MinGW installations.

```powershell
.\tools\windows_deploy.ps1 -QtDir "E:\Qt\5.15.2\mingw81_64" -MingwDir "E:\Qt\Tools\mingw810_64"
```

The script builds the GUI, collects its runtime dependencies and Qt plugins, and writes the package to `dist\`. It then launches the packaged executable to check that it starts.

## License

The NMRduino GUI is licensed under [GPL-3.0](LICENCE).

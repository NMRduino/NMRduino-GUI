# NMRduino GUI

NMRduino GUI is the desktop software for the [NMRduino spectrometer](https://github.com/NMRduino/NMRduino). It includes three tools for running experiments: Spectrum Analyzer, Pulse and Acquire, and Task Scheduler.

## A. Downloads

Version **3.40** is available as source. The Windows binary linked below is **3.37.4**. See the [change log](CHANGELOG.md) for the new features. The Windows package is ready to run. On a Raspberry Pi, you can build the GUI from source using the commands in section C.

| Platform | Download | Instructions |
| --- | --- | --- |
| Windows (64-bit) | [Download the compiled Windows ZIP (3.37.4)](https://github.com/NMRduino/NMRduino-GUI/releases/download/3.37.4/nmrduino-3.37.4-win64.zip) | [B. Run on Windows](#b-run-on-windows) |
| Raspberry Pi, including Pi 400 | [Download the source code (.tar.gz)](https://codeload.github.com/NMRduino/NMRduino-GUI/tar.gz/refs/tags/3.40) | [C. Build on Raspberry Pi](#c-build-on-raspberry-pi) |

For other versions, see [all GUI releases](https://github.com/NMRduino/NMRduino-GUI/releases). If you also need firmware for the Teensy, see [D. Connect the spectrometer](#d-connect-the-spectrometer).

## B. Run on Windows

1. Download the **compiled Windows ZIP** from the table above.
2. Right-click the downloaded file, choose **Extract All**, and open the extracted folder.
3. Double-click `nmrduino.exe` to start the GUI.

The ZIP includes Qt and the other files the GUI needs. If you move the program, move the whole extracted folder so these files stay together.

## C. Build on Raspberry Pi

These instructions are for Raspberry Pi OS with desktop, including on the Raspberry Pi 400. If you are preparing a new SD card, use [Raspberry Pi Imager](https://www.raspberrypi.com/software/) and choose a username and password under **Customisation → User**. You will use that password when `sudo` prompts for it.

### C1. Install the build tools

Open a terminal on the Pi and run the following commands. This installs the compiler, Qt 5, and the serial-port library used to communicate with the spectrometer.

```bash
sudo apt update
sudo apt install build-essential curl qt5-qmake qtbase5-dev libqt5serialport5-dev libqt5svg5
```

### C2. Download the source code

You can do this from the same terminal. The commands below download the source archive linked in section A and extract it into your home directory.

```bash
cd ~
cd ~ && curl -fL --retry 2 -o nmrduino-3.40.tar.gz https://codeload.github.com/NMRduino/NMRduino-GUI/tar.gz/refs/tags/3.40 && tar -xzf nmrduino-3.40.tar.gz
```

### C3. Compile the GUI

Run the following commands and wait for the build to finish. This step can take several minutes on a Pi.

```bash
mkdir -p ~/NMRduino-GUI-3.40/build
cd ~/NMRduino-GUI-3.40/build
/usr/lib/qt5/bin/qmake ../src/NMRduino.pro CONFIG+=release && make
```

### C4. Start the GUI

Once the terminal prompt returns after a successful build, run:

```bash
~/NMRduino-GUI-3.40/build/nmrduino
```

You only need to compile the GUI once. The next time you want to use it, open a terminal and run that last command.

## D. Connect the spectrometer

The Teensy requires NMRduino firmware **1.2.0 or later**. If you need to install or update the firmware, open the [latest NMRduino firmware release](https://github.com/NMRduino/NMRduino/releases/latest) and download the `.hex` file listed under **Assets**. Use [Teensy Loader](https://www.pjrc.com/teensy/loader.html) to flash it to the board.

Connect the NMRduino by USB before starting the GUI. If one device matches the configured vendor and product IDs (defaults 5824 and 1155), the GUI connects and displays its serial number. If several devices match, choose the serial number in the **NMRduino** side panel and click **Reconnect**. Select **NMRduino** as the acquisition device for your experiment.

You can also try the software before connecting a spectrometer. Open **Spectrum Analyzer** and select **Test DAQ** to use the simulated device.

To connect a Twinleaf CSB current supply, open **File → Add a device → Twinleaf CSB** and select its serial number. The list shows available serial ports with USB vendor ID 1155 and product ID 22336. Once the port opens, the GUI displays the CSB panel.

## E. Settings and logs

The GUI stores settings and logs in `%USERPROFILE%\.NMRduino\` on Windows and `~/.NMRduino/` on Raspberry Pi. If you prefer to keep them with the program, place a `_config.ini` file alongside the executable to enable portable mode.

On Raspberry Pi, the default working directory is the current user’s configured Desktop folder under `data/yyMMdd`. Pulse and Acquire opens its file chooser in `~/PulseSequences`, which the GUI creates at startup. The default Pulse and Acquire timeout is 60 seconds.

## F. Build and package a Windows release

If you want to build the Windows version yourself, open `src/NMRduino.pro` in Qt Creator, select a Qt 5.15.2 kit with a C++14 compiler, and build in **Release** mode. The MinGW 64-bit kit statically links the MinGW runtime. MSVC is also supported.

To create a distributable ZIP with MinGW, run the following command from the repository root in PowerShell. Adjust the two paths to match your Qt and MinGW installations.

```powershell
.\tools\windows_deploy.ps1 -QtDir "E:\Qt\5.15.2\mingw81_64" -MingwDir "E:\Qt\Tools\mingw810_64"
```

The script builds the GUI, collects its runtime dependencies and Qt plugins, and writes the package to `dist\`. It then launches the packaged executable to check that it starts.

## G. License

NMRduino GUI is licensed under [GPL-3.0](LICENCE).

To build the GUI, use the open-source Qt 5.15 development packages. The Qt modules used by NMRduino GUI are available under open-source terms compatible with GPLv3. On Raspberry Pi OS, the packages listed in section C1 are sufficient. An educational or commercial Qt licence is optional.

The bundled QCustomPlot 2.0.1 is licensed under GPLv3 or later. When redistributing binaries that include Qt libraries or plugins, include the applicable licence texts and notices and provide access to the corresponding source for the versions shipped. See [Qt’s open-source licensing guidance](https://www.qt.io/development/open-source-lgpl-obligations) and the [QCustomPlot licence notice](src/plots/qcustomplot.h).

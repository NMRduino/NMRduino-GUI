# NMRduino GUI changelog

## 3.40

- Fixed delayed Twinleaf CSB console replies. Responses now appear as data arrives, including after a single command. Verified on Raspberry Pi with a Twinleaf CSB R1. See [issue #3](https://github.com/NMRduino/NMRduino-GUI/issues/3).
- **File → Add a device → Twinleaf CSB** lists matching serial numbers and port names. Selecting a device opens its port and displays its panel. If the port fails to open, the GUI reports the operating system’s error.
- Corrected the CSB USB defaults to VID 1155 (`0x0483`) and PID 22336 (`0x5740`). Saved CSB settings with the two IDs reversed are corrected when loaded.

## 3.39

- NMRduino appears first in the device panel. At startup, one matching USB device connects automatically and the GUI displays its serial number. With multiple matches, choose a serial number in the panel and click **Reconnect**.
- Moved Timeout into the Pulse and Acquire settings and changed its default to 60 seconds. Acquisition State now appears in the selected device’s settings.
- Moved the cursor controls and coordinate readouts inside the time-domain and spectrum plots to give the plots more space.
- The pulse-sequence time axis switches to microseconds at spans of 6 ms or less. At maximum zoom the divisions are 1 µs apart.
- The GUI creates `~/PulseSequences` and uses it as the starting folder for **Open** in Pulse and Acquire.
- On Raspberry Pi, dated acquisition folders use the current user’s configured Desktop location, including localized folder names. This fixes directory creation on systems with a username other than `pi`.

## 3.38

Changes since 3.37.4.

- Added measurement cursors to the time trace and spectrum in Pulse and Acquire and Spectrum Analyzer. Dotted crosshairs follow the GUI theme. Left-click to set or move a reference and read signed Δx and Δy. Right-click to clear it. See [issue #4](https://github.com/NMRduino/NMRduino-GUI/issues/4).
- Spectra open at 0–100 mV/√Hz with a maximum of 5000 mV/√Hz. Edit `StartupAmplitude` and `MaxAmplitude` under `[SpectrumPlot]` in `_config.ini` to change these values. The cap also applies on logarithmic axes. See [issue #2](https://github.com/NMRduino/NMRduino-GUI/issues/2).
- Set the default Teensy USB identifiers to vendor 5824 (0x16C0) and product 1155 (0x0483). Existing explicitly configured identifiers take precedence.
- Pulse and Acquire and Task Scheduler default to `./trash/` when the data directory is empty or missing. The path is relative to the GUI's working directory. Choose a named data directory for runs you want to retain.
- Hid the Mode label and selector in Pulse and Acquire. The acquisition mode is still loaded from the task settings.

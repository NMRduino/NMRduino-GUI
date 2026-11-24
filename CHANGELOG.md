# NMRduino GUI 3.38

Changes since 3.37.4.

- Added measurement cursors to the time trace and spectrum in Pulse and Acquire and Spectrum Analyzer. Dotted crosshairs follow the GUI theme. Left-click to set or move a reference and read signed Δx and Δy. Right-click to clear it. See [issue #4](https://github.com/NMRduino/NMRduino-GUI/issues/4).
- Spectra open at 0–100 mV/√Hz with a maximum of 5000 mV/√Hz. Edit `StartupAmplitude` and `MaxAmplitude` under `[SpectrumPlot]` in `_config.ini` to change these values. The cap also applies on logarithmic axes. See [issue #2](https://github.com/NMRduino/NMRduino-GUI/issues/2).
- Set the default Teensy USB identifiers to vendor 5824 (0x16C0) and product 1155 (0x0483). Existing explicitly configured identifiers take precedence.
- Pulse and Acquire and Task Scheduler default to `./trash/` when the data directory is empty or missing. The path is relative to the GUI's working directory. Choose a named data directory for runs you want to retain.
- Hid the Mode label and selector in Pulse and Acquire. The acquisition mode is still loaded from the task settings.

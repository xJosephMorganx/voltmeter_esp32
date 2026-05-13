# Voltmeter Web Mockup

This folder contains a browser-only mockup of the ESP32 voltmeter interface. It is meant for visual iteration without flashing or compiling the ESP32 firmware.

## Run

Open `index.html` directly in a browser.

Optional local server:

```bash
cd mockup
python -m http.server 8080
```

Then open <http://localhost:8080>.

## Notes

- The mockup does not call `/api/voltage`.
- Voltage and Raw ADC values are simulated in `script.js`.
- Design changes can be made here first, then copied back into the embedded header files when approved.

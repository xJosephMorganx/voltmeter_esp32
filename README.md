# ESP32 Web Voltmeter

An ESP32-based voltmeter with a browser dashboard. The board reads an analog voltage through its ADC, hosts a small web server over Wi-Fi, and shows the measurement with a responsive gauge, raw ADC value, connection status, and light/dark theme support.

The web page is embedded directly in program memory, so the project does not require SPIFFS, LittleFS, an external server, or a frontend build step.

## Preview

![ESP32 Web Voltmeter dashboard](assets/web-preview.png)

## Features

- Real-time voltage readings from an ESP32 ADC pin.
- Animated semicircular gauge with smooth needle movement.
- Raw ADC value display.
- Configurable display range: `3.3 V`, `5 V`, `12 V`, or custom.
- Light and dark mode toggle with local browser preference storage.
- Offline simulation mode when the browser cannot reach the ESP32 API.
- Self-contained web server using the Arduino `WebServer` library.
- HTML, CSS, and JavaScript separated into dedicated header files.

## Hardware Requirements

| Component | Notes |
| --- | --- |
| ESP32 development board | Any common ESP32 board with ADC-capable pins should work. |
| USB cable | Used for programming, serial output, and power. |
| Analog voltage source | Keep the ADC input within the safe ESP32 range. |
| Jumper wires | Used for basic signal and ground connections. |
| Potentiometer | Optional, useful for testing variable voltage readings. |
| Voltage divider or signal conditioning circuit | Required when measuring more than `3.3 V`. |

## Software Requirements

- Arduino IDE.
- ESP32 board package for Arduino IDE.
- A browser connected to the same Wi-Fi network as the ESP32.

## Project Structure

```text
VoltmeterESP32/
├── VoltmeterESP32.ino
├── index_html.h
├── style_css.h
├── script_js.h
├── assets/
│   └── web-preview.png
├── LICENSE
└── README.md
```

| File | Purpose |
| --- | --- |
| `VoltmeterESP32.ino` | Main Arduino sketch with Wi-Fi setup, ADC reading, HTTP routes, and the main loop. |
| `index_html.h` | Embedded HTML served at `/`. |
| `style_css.h` | Embedded CSS served at `/style.css`. |
| `script_js.h` | Embedded JavaScript served at `/script.js`. |
| `assets/web-preview.png` | Screenshot used in the Preview section. |
| `LICENSE` | Project license. |

## Wiring

Basic wiring for a signal that is already safe for the ESP32 ADC:

```text
Signal to measure  -> GPIO34
Circuit ground     -> GND
```

Important: do not connect more than `3.3 V` directly to an ESP32 ADC pin. Higher voltages can permanently damage the board.

For voltages above `3.3 V`, use a voltage divider or another signal conditioning circuit:

```text
Measured voltage -> R1 -> ADC pin -> R2 -> GND
```

Choose resistor values so the ADC pin never receives more than `3.3 V`.

## Default ADC Configuration

The default ADC pin is `GPIO34`:

```cpp
const int ADC_PIN = 34;
```

`GPIO34` is input-only and commonly used for analog readings on ESP32 boards.

The sketch configures 12-bit ADC readings and `ADC_11db` attenuation:

```cpp
analogReadResolution(12);
analogSetPinAttenuation(ADC_PIN, ADC_11db);
```

The voltage calculation uses these constants:

```cpp
const float ADC_REFERENCE_VOLTAGE = 3.3;
const int ADC_MAX_READING = 4095;
```

If you use a voltage divider, update the conversion logic so the displayed voltage represents the original measured voltage, not only the voltage present at the ADC pin.

## Wi-Fi Setup

Open `VoltmeterESP32.ino` and update the Wi-Fi credentials:

```cpp
const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";
```

The ESP32 and the browser must be connected to the same network.

## Uploading With Arduino IDE

1. Install the ESP32 board package in Arduino IDE.
2. Open `VoltmeterESP32.ino`.
3. Select the correct ESP32 board.
4. Select the correct USB port.
5. Update `WIFI_SSID` and `WIFI_PASSWORD`.
6. Upload the sketch.
7. Open the Serial Monitor at `115200 baud`.
8. Wait for the ESP32 to print an address like this:

```text
Server ready: http://192.168.1.50
```

Open that address in a browser connected to the same Wi-Fi network.

## How It Works

The ESP32 starts a web server on port `80` and exposes these routes:

| Route | Description |
| --- | --- |
| `/` | Serves the dashboard HTML. |
| `/style.css` | Serves the dashboard styles. |
| `/script.js` | Serves the dashboard behavior. |
| `/api/voltage` | Returns the latest ADC reading as JSON. |

The browser loads the dashboard from the ESP32 and polls `/api/voltage` every `700 ms`. Each response updates the numeric voltage, raw ADC value, gauge arc, and needle position.

If the browser cannot reach the API, the dashboard switches to simulation mode so the interface can still be previewed.

## API Response

Request:

```http
GET /api/voltage
```

Example response:

```json
{
  "raw": 2048,
  "voltage": 1.65,
  "maxVoltage": 3.3
}
```

| Field | Description |
| --- | --- |
| `raw` | Raw ADC reading from `0` to `4095`. |
| `voltage` | Calculated voltage at the ADC pin. |
| `maxVoltage` | ADC reference voltage used by the sketch. |

## Customization

To change the ADC pin, update this constant:

```cpp
const int ADC_PIN = 34;
```

To adjust the basic ADC conversion, update these constants:

```cpp
const float ADC_REFERENCE_VOLTAGE = 3.3;
const int ADC_MAX_READING = 4095;
```

To edit the web interface:

- Modify `index_html.h` for page structure.
- Modify `style_css.h` for visual styling.
- Modify `script_js.h` for gauge behavior, polling, theme handling, and simulation mode.

The dashboard range selector only changes the visual gauge range. It does not make the ESP32 ADC pin safe for higher voltages.

## Troubleshooting

| Problem | What to check |
| --- | --- |
| The Serial Monitor never shows an IP address. | Confirm the Wi-Fi name and password, then verify the ESP32 is within range of the router. |
| The browser cannot open the dashboard. | Make sure the browser device is on the same network as the ESP32. |
| The dashboard shows `Simulation`. | The browser loaded the page but cannot reach `/api/voltage`; refresh the page and check the ESP32 IP address. |
| The voltage looks inaccurate. | Check the ADC input voltage with a multimeter and calibrate the conversion constants or voltage divider formula. |
| Readings jump around. | Use short ground connections, avoid floating inputs, and add signal conditioning if the measured source is noisy. |

## License

This project is released under the GNU General Public License v3.0.

The GPL-3.0 license requires derivative works to remain open source under the same license terms. This helps keep improvements available to the community.

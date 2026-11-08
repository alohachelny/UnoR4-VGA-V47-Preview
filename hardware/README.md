# Hardware Connections & Pinout

## VGA Connector Pinout (DE-15)
For a standard VGA cable, you only need 5 pins for monochrome output:
*   **Pin 1 (Red):** Not connected (or tie to Green)
*   **Pin 2 (Green):** Connect to Arduino D11 via 470-520Ω resistor.
*   **Pin 3 (Blue):** Not connected (or tie to Green)
*   **Pin 13 (HSync):** Connect to Arduino D9. Connect to Arduino D9 via 470-680Ω resistor.
*   **Pin 14 (VSync):** Connect to Arduino D8. Connect to Arduino D8 via 470-680Ω resistor.
*   **Pins 5, 6, 7, 8, 10 (GND):** Connect to Arduino GND.

## Arduino Uno R4 Minima Pinout

| Arduino Pin | RA4M1 Port | Function | Description |
| :--- | :--- | :--- | :--- |
| **D9** | P303 | HSync | Hardware timer output (GPT7). |
| **D8** | P304 | VSync | Software GPIO. |
| **D11** | P109 | Video | SPI1 MOSI (Data output). |
| **D2** | P105 | Button 1 | Adjust line phase (BEG +). |
| **D3** | P104 | Button 2 | Adjust line phase (BEG -). |
| **D5** | P102 | Button 3 | Shift window left. |
| **D6** | P106 | Button 4 | Shift window right. |

## Schematic Notes
*   **Resistors:** It is highly recommended to put a **470 Ohm** resistor in series with the Video (Green) line to protect the monitor from overvoltage (Arduino outputs 3.3V/5V, VGA expects 0.7V peak-to-peak).
*   **Ground:** Ensure a solid, short wire between the Arduino GND and the VGA connector GND to minimize noise (especially at 24 MHz SPI speeds).

### Band Module 

# GuardianDrive – Biometric Health-Monitoring Wearable Band

The GuardianDrive Wearable Band is an embedded IoT wristband developed to continuously capture driver vital signs, stream biometric telemetry over Bluetooth Low Energy (BLE), and enforce pre-drive authorization for accident prevention.

---

## Hardware Architecture & Components

* **MCU:** ESP32-C3 Super Mini (low-power 32-bit RISC-V microcontroller with integrated 2.4 GHz Wi-Fi & BLE 5.0)
* **Photoplethysmography (PPG) Sensor:** MAX30102 (measures Heart Rate and SpO₂)
* **Infrared Temperature Sensor:** MLX90614 (non-contact object/body temperature sensing)
* **Power Management:** HW-357 Li-ion charging & boost converter module + rechargeable 18650 Li-ion battery
* **Custom PCB:** Designed in Altium Designer, fabricated with dedicated power planes and I2C routing tracks
* **Enclosure:** Custom-molded 3D-printed PLA housing

<img width="1650" height="1275" alt="image22" src="https://github.com/user-attachments/assets/0cb6625c-f7e7-48a9-a319-e00693c5f45e" />

<img width="421" height="514" alt="image55" src="https://github.com/user-attachments/assets/0b549ea5-ddc4-430d-ac20-85101655ccd7" />


<img width="363" height="483" alt="image94" src="https://github.com/user-attachments/assets/93c8a4b1-8cbc-49fa-b9d3-bebccfb507f6" />

---

## Firmware Features & Signal Processing

* **I2C Sensor Integration:** Simultaneous non-blocking data acquisition from MAX30102 and MLX90614 over I2C buses.
* **Firmware Sliding Window Average:** Mitigates high-frequency noise and motion artifacts common in PPG signals before dispatching payloads.
* **Empirical Temperature Calibration:** Applies fixed compensation offsets in firmware to calibrate the MLX90614 against benchmark reference thermometers.
* **BLE GATT Service:** Advertises health telemetry characteristics and streams real-time biometric readings (BPM, SpO₂, temperature) directly to the GuardianDrive mobile app.
* **Hardware Self-Test & Diagnostics:** Executes an automated component integrity check upon power-up and responds to mobile pre-drive verification requests.
* **Dynamic Power States:** Supports low-power sleep modes triggered when the band detects improper wearing or receives emergency sleep commands from the mobile app.


---

## Setup & Flashing Instructions

1. **Prerequisites:** Install [Arduino IDE](https://www.arduino.cc/en/software) or PlatformIO inside VS Code.
2. **Board Support:** Add the ESP32 board index:
   ```text
   [https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json](https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json)

---

3. **Libraries:**
Install `SparkFun MAX3010x`, `Adafruit MLX90614`, and default `ESP32 BLE Arduino` libraries.

--

5. **Compile & Upload:**
* Connect the ESP32-C3 Super Mini via USB-C.
* Select **ESP32C3 Dev Module** from the board manager.
* Verify COM port and click **Upload**.



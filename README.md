# Health_Monitoring_Steering_Wheel
Complete ESP32-based version of the Health Monitoring Steering Wheel / Seatbelt System — with sensor connections, power tips, and full working code

**Disclamer:** Use the Below Image just for refrence do not follow the connection as per the image use the connection that i have provided in text form.

![project](https://github.com/user-attachments/assets/f9c0d093-3809-4d5a-97f6-cc26ed1de95a)


## ⚙️ Components List (ESP32-Compatible)

| Component             | Use                          | ESP32 Pin Connection                           |
| --------------------- | ---------------------------- | ---------------------------------------------- |
| **MAX30100/MAX30102** | Heart rate / SpO₂ sensor     | SDA → GPIO21, SCL → GPIO22                     |
| **AD8232**            | ECG Signal                   | OUT → GPIO34 (Analog Input)                    |
| **SIM800L**           | GSM SMS Alert Module         | TX → GPIO16, RX → GPIO17 (via voltage divider) |
| **Relay Module**      | Hazard light / Slow-down car | IN → GPIO27                                    |
| **GPS (Neo-6M)**      | Location for alert           | TX → GPIO4, RX → GPIO5                         |
| **Power**             | 5V 2A (SIM800L needs it)     | External power or LiPo                         |

---

## 🔌 SIM800L Power Warning

**DO NOT** power SIM800L from ESP32’s 3.3V.
Use:

* **External 5V 2A power bank**
* Or **TP4056 + 18650** battery + step-down converter (AMS1117)

---

## 🧠 Working Logic

* Monitor HR & ECG continuously.
* If abnormal vitals for 5 seconds:

  * Activate relay to slow down car.
  * Send SMS alert (with location if GPS available).

---

## ✅ Features:

* Reads **real-time HR + ECG**.
* Triggers **hazard light or motor relay**.
* Sends **SMS alert with location** (if GPS found).
* Handles false positives with delay logic.

---

## 🛠 Setup Notes:

* Secure ECG pads on seatbelt or grip area.
* Embed MAX3010x on steering for HeartRate.
* Secure ESP32 + modules under dash or seat.
* Use fused power for SIM800L.

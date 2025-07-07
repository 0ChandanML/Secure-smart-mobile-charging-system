# Secure-smart-mobile-charging-system
ESP32-based secure mobile charger with coin slot and fingerprint access.
Here’s a complete and polished `README.md` file for your **Secure Smart Mobile Charging System** project, ready to copy-paste into your GitHub repository:

---

```markdown
# 🔒 Secure Smart Mobile Charging System 🔋

A secure, user-authenticated mobile charging system using **ESP32**, **fingerprint sensor**, and a **coin acceptor**. Designed for use in public spaces such as malls, railway stations, colleges, and airports — where device safety and controlled charging are essential.

---

## 📌 Features

- 🔐 **Biometric Security** – Fingerprint authentication for both deposit and retrieval
- 💰 **Coin-Based Payment** – ₹5 activation per charge session
- ⚡ **Timed Charging** – Automatic cutoff after 60 seconds
- 🔊 **Buzzer Alerts** – For timeout or unauthorized access
- 📟 **LCD Display** – Real-time instructions and system messages
- 🔁 **Self-Resetting System** – Ready for next user after session
- ☀️ **Solar + Battery Compatible** – For uninterrupted power in remote areas

---

## 🛠️ Tech Stack

### Hardware:
- ESP32 DevKit V1
- R307 / GT511C3 Fingerprint Sensor
- Coin Acceptor (CH-926 or RM5)
- 16x2 I2C LCD
- Servo Motor (SG90 or MG996R)
- Relay Module
- Buzzer
- Solar Panel + Battery (optional)
- 8x8 Dot Matrix Display (for advertising)

### Software:
- Arduino IDE
- Embedded C
- Adafruit Fingerprint Library
- LiquidCrystal_I2C Library
- ESP32Servo Library

---

## 📷 System Overview

### 🧾 Report
The complete Final Year Project Report is available here:  
📄 [`Project_Report_SecureSmartCharger.pdf`](./docs/Project_Report_SecureSmartCharger.pdf)

### 🧠 Block Diagram
> ESP32 → Coin Acceptor → Fingerprint Sensor → Locker (Servo Motor) → Charger (Relay) → LCD/Buzzer Output

### 🖼️ Example Screenshots
> _(Add hardware images, fingerprint sensor interaction, LCD messages, etc.)_

---

## 🚀 How It Works

1. **Insert ₹5 Coin**
2. **Enroll Fingerprint** (for this session)
3. **Locker Unlocks** – Place your device inside
4. **Charging Begins**
5. **Timeout or Fingerprint Match** to retrieve device
6. **System Resets for Next User**

---

## 📦 Folder Structure

```

SecureSmartMobileCharger/
├── src/
│   └── main.ino
├── docs/
│   └── Project\_Report\_SecureSmartCharger.pdf
├── images/
│   └── (screenshots, diagrams)
├── README.md
└── .gitignore

```

---

## 🔮 Future Enhancements

- UPI / QR / NFC Payment Integration
- Face Recognition / RFID Authentication
- Mobile App for Slot Booking & Notifications
- IoT Cloud Dashboard for Monitoring
- Support for Fast Charging & USB-C
- Solar-powered Backup

---

## 👨‍💻 Contributors

- **Chandan M L** – 4SM22EC404  
- **Chandana U** – 4SM22EC405  
- **Karthik N** – 4SM22EC410  
- **Priyanka N** – 4SM22EC419  

---

📍 *Dept. of E&CE Engineering, SJMIT, Chitradurga – 2024-25*


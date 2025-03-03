# smartlamp-ESP32

## Why This Exists

Tired of getting out of bed to adjust my lamp. Built this to:
- Control my lamp from phone/PC
- Set up custom wake-up routines
- Automate lighting based on time of day
- Eliminate the physical remote from my life

## How It Works

This project uses an ESP32 microcontroller to capture and replay infrared signals that control my specific bedroom lamp. The ESP32 hosts a simple web server that presents a control interface accessible from any device on the same network.

## Important Note

**This is designed for my specific lamp model**. Every lamp has their own IR codes. You'll need to modify the `commandToCode` map with your own lamp's specific IR codes if you want to use this for a different lamp.

## Hardware Requirements

- ESP32 development board
- IR receiver (connected to pin 27)
- IR transmitter (connected to pin 33)
- USB power supply

## Setup

1. Set environment variables for your WiFi:
   ```
   export SSID="your_wifi_name"
   export PASSWORD="your_wifi_password"
   ```

2. Flash the code to your ESP32

3. Check the serial monitor for the IP address

4. Navigate to that IP address in a browser to control your lamp

## Usage

Once set up, you'll see a simple interface with buttons for:
- Off/On
- Brighter/Darker
- Colder/Warmer
- Timer
- Sleep
- Study

Each button sends the corresponding IR code to your lamp.

## Customization

To adapt this for your own lamp:
1. Use the IR receiver to capture your lamp's codes (check serial output)
2. Update the `commandToCode` map with your lamp's specific codes
3. Rename buttons to match your lamp's functions

## Future Improvements

- Add scheduling capabilities
- Integrate with Home Assistant
- Add voice control
- Support for multiple IR devices

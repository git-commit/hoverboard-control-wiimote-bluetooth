# ATTENTION: THIS CONTROL METHOD WAS HASTELY COPY-PASTED TOGETHER AT 36C3 JUST TO MAKE SOMETHING WORK; IT IS NOT STABLE IF UART COMMUNICATION BREAKS; BE CAREFUL AND DON'T HURT YOURSELF

# Hoverboard Control from Wiimote/Nunchuck inputs via UART
related hoverboard firmware: https://github.com/git-commit/hoverboard-firmware-little-dipper

# Features
* Wiimote and Nunchuk Controls for your hoverboard hack
* Dead Man's Grip on *C* Button
* Boost Mode on *Z* Button
* Automatic stopping if Wiimote connection breaks

# Usage
* Power up hoverboard
* Press 1+2 on Wiimote to pair
* LED 1 on Wiimote should light up
* Pull the Nunchuk plug out and plug it back in
* Press *C* and control the hoverboard using the Nunchuk control stick

# Connection
* Flash your ESP32
* Connect 3.3v from the hoverboard STM32, Ground
* Connect TX to Pin 25 and RX to Pin 26


# Dependencies
* [Wiimote.h](https://github.com/takeru/Wiimote "Wiimote.h")

# Hardware
* ESP32
* Wiimote + Nunchuk
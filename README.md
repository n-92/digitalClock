# digitalClock based on Waveshare_ST7262_LVGL with esp32s3 as the main CPU
![How it looks!](https://ibb.co/p9FvG4w)

# Waveshare_ST7262_LVGL

Waveshare_ST7262_LVGL is an Arduino library designed for [Waveshare ESP32-S3-Touch-LCD-7](https://www.waveshare.com/wiki/ESP32-S3-Touch-LCD-7) using ESP32-S3 SOC with 7 inch ST7262 LCD and GT911 touchscreen to facilitate rapid GUI development using LVGL.

## Dependencies
| Library  	|   Version	| 
|---	|---	|
| [LVGL](https://github.com/lvgl/lvgl) 	|  >=8.3.0 && <=8.3.11 	| 
|   [ESP32_Display_Panel](https://github.com/esp-arduino-libs/ESP32_Display_Panel)	|  >=0.1.4 	|  
|   [ESP32_IO_Expander](https://github.com/esp-arduino-libs/ESP32_IO_Expander/)	|  0.0.3 	|  

## Usage
 * Clone the entire repository and put all the files in a folder with name `digitalClock` if it doesn't already exist. 
 * Use the Latest Arduino IDE to open the .ino file and upload to the target device connected on UART port.
 * Tap on the minutes and seconds text to adjust. Similiary, tap on the date to adjust. 

## Acknowledgements
Example code was sourced from Waveshare Wiki Page mentioned below. I just made it easier to implement in any sketch file.
 - [Waveshare - [WIKI] ESP32-S3-Touch-LCD-7](https://www.waveshare.com/wiki/ESP32-S3-Touch-LCD-7)

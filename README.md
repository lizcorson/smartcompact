# smartcompact
This is a tiny, portable smart mirror.

## The Goal
I want to make a battery-powered, WiFi-connected smart mirror that you can carry around with you. The first step will be to make it work just using a WiFi connection, but my ultimate goal is to connect over bluetooth to a phone app which would send weather data based on the phone's location, so it would truly be portable.

## Status
I have the OLED display working and displaying test data. Thanks to Daniel Eichhorn (@squix78) for the code to display weather icons on the SSD1306 OLED. Right now it's just connected to an Uno since I don't want to solder it to my Feather Huzzah yet.

![alt tag](https://github.com/lizcorson/smartcompact/blob/master/photos/oled demo.jpg)

Since Weather Underground wasn't working for me, I found an alternate weather API at worldweatheronline.com which sends a nice, simple CSV-formatted result. I have formatted it to get almost all the data I want. I still need to figure out how to calculate and display chance of precipitation.

![alt tag](https://github.com/lizcorson/smartcompact/blob/master/photos/weather print example.PNG)

Previously:

I'm having some trouble pulling all the data I need from Weather Underground. I need to call both conditions and forecast as two separate API calls, which I think is causing problems. I have two functions: getCurrentWeather and getFutureWeather, one for each API call. If I comment out one, I can run the other just fine, but not both, and I don't know why.

Also, the forecast API call result is so huge that trying to parse it with ArduinoJson causes a memory overflow, so for now I just use some simple string manipulation.

I initially planned to use forecast.io, but that is https only.

## Parts used
- Adafruit Feather Huzzah https://www.adafruit.com/products/2821
- LiPo battery https://www.adafruit.com/products/2750
- 0.96" 128x64 I2C White OLED screen http://www.ebay.com/itm/191785893008

## Thanks
to the following projects for inspiration, tips, etc.

- Daniel Eichhorn's ESP8266 Weather Station: https://github.com/squix78/esp8266-projects/tree/master/arduino-ide/weather-station-v2
- Tecywiz121's ArduinoWeatherClient: https://github.com/tecywiz121/ArduinoWeatherClient
Huertomato Arduino Firmware
========
This is the Arduino firmware for the Huertomato hydroponics controller.

The main project page is located [here.](http://thegreenautomation.com)

Huertomato is an Arduino based controller for the management of hydroponics systems. It has all the sensors you’d need to monitor your plant’s environment and nutrient tank and can water at the times you choose.

![alt tag](http://thegreenautomation.com/wp-content/uploads/2015/04/htmt.png)

[Main unit:](https://github.com/TheGreenAutomation/Huertomato_MainHW)
* 3.2″ Touchscreen LCD for system status and interaction.
* DS18B20 temperature sensor.
* DHT11 humidity sensor.
* Photoresistor for light level.
* Real Time Clock with backup battery.
* Relay for an AC waterpump.
* RGB LED and buzzer for status.
* 3x RJ45 I/O ports.

[Reservoir sensing module:](https://github.com/TheGreenAutomation/Huertomato_NutrientHW)
* Atlas Scientific pH sensor circuit
* Atlas Scientific EC sensor circuit
* HC-SR04 sonar sensor for measuring nutrient tank level.

Features:
========
* Monitor temperature, humidity, light, pH, EC and tank level.
* Plug your pump into Huertomato, set watering timers and forget.
* Automatically stop watering at night.
* Log all sensor data to SD Card.
* All readings are temperature-compensated.
* Trigger alarms if sensor readings are unsafe for your plants.
* Pump auto shut off protection if tank level is critically low.
* Fully graphical sensor calibration.

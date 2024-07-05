# IoT-based-Hydroponic-System
# Overview
Growing plants in the soil can be challenging as the land for cultivation is decreasing rapidly due to high population. In this scenario we need a soil-less cultivation method that can fulfill our needs. The aim of this project is to design and construct an automatic hydroponic system that does not depend on the outside climate. The designed system is capable of growing common types of crops that can be used as a food source inside homes. The system's design was created by evaluating various hydroponic systems to determine their compatibility for automation. A microcontroller served as the system's central processor, connecting with various sensors to regulate system settings and reduce the need for human involvement. An IoT platform was utilized for storing and showcasing system parameters and graphical interface for accessing remotely. The system can sustain optimal plant growth conditions with little user intervention. The confirmation of the entire system's functionality was achieved by assessing the responses of individual system parts and tracking them on the IoT platform. This system offers real-time notifications to alert the hydroponic system user when the reservoir’s water level is low as well as the status of the system. Additionally, the system offers a vast array of data that may be crucial for plant researchers and enhances comprehension of how the main factors of the hydroponic system relate to plant development. The platform being suggested can be utilized for quantitatively improving the configuration of indoor/outdoor farming as well as for automating some of the most labor-intensive maintenance tasks. Furthermore, this monitoring system has the potential to be utilized for making important decisions at a higher level, once a sufficient amount of data has been gathered. As mentioned, the system does not depend on the outside climate, therefore we can grow summer vegetables in winter also.
# Components
* ESP8266/ESP32 (microcontroller)
* 5V relay module
* pH sensor
* TDS sensor
* Temperature sensor (DHT11)
* Ultrasonic sensor (HC-SR04)
* Submersible pump
* Dozing pump
* Exhaust fan
* Grow light
* Air pump
# Features
Our proposed hydroponic system has many features like -

![Picsart_24-06-14_17-52-49-342](https://github.com/SubhadipSR/IoT-based-Hydroponic-System/assets/70135413/693c48ce-2456-4579-b9cb-4b1dfc1dc645)

* Automatic pH and TDS value change according to the needs.
* Automatic nutrients mix delivery.
* Automatic light on-off based on the current timing.
* Automatic aeration system for the polyhouse based on the temperature.
* Automatic air pump activation.
* Monitor and control the system remotely through Blynk IoT platform.
# How to run this project
* Install Arduino IDE and set the environment for ESP8266/ESP32.
* Combine all the sensors and actuators and connect with the microcontroller.
* Change the thresholds according to your need.
* Create a Blynk template and set up the web dashboard and add datastreams.
* Upload the code into the microcontroller.
* Power up the microntroller, sensors and actuators.
* And congratulations, you have done it. :)
![PXL_20240418_063912559](https://github.com/SubhadipSR/IoT-based-Hydroponic-System/assets/70135413/0ef81776-c0a0-4594-8ffe-1f5c092b2952)

** If you are using ESP8266, then you have to take 2 microcontrollers as it has only 1 analog pin and we need atleast 2 analog pins. You can use Arduino UNO, but for WiFi connection, you have to buy a WiFi module. My suggestion is that, you should use ESP32 module for this project.
  

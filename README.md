# IoT-IA1-2021 - Plant Monitor System
## Introduction
The goal of this project is to monitor a plants conditions, to see if necessary needs, like light and temperature, are met. To accomplish this goal two types of sensor for each plant are used: a photoresistor and a TMP36 temperature sensor to be able to read the luminosity and the temperature room. In case of allarming values a led or a buzzer will be turned on to notify the owner. The led and the buzzer are one for each plant.\
Depending on the values there are three states:
- Normal: plant's conditions are met.
- Allarming: the plant's conditions are not optimal but they don't require owner intervention. The led will be turned on
- Critical: the plant's conditions are critical and require the owner to take action. The buzzer will start beeping.
The microcontroller read and sends data each second.
## Components
The TMP36 temperature sensor has an ±2°C accuracy over temperature and a 10 mV/°C scale factor.
## Architecture
Each microcontroller read values from the sensor and transmitts them using MQTT-SN to a gateway that will act both as MQTT-SN broker, MQTT broker and transparent bridge between the two. Finally the values will be sent to the cloud backend. The owner will have the possibility to view the data sent to the backend via a website.\
As microcontroller a nucleo-f410re will be used running RIOT OS sending and receiving through ethos. The nucleo-f410re will send data to the gateway running rsmb and mosquitto that they act as MQTT-SN/transparent bridge and MQTT broker. Mosquitto then connects to the AWS IoT Core that will send the data that receives to a S3 public bucket.

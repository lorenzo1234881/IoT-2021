# IoT-IA1-2021 - Plant Monitor System
## Introduction
The goal of this project is to monitor a plants conditions, to see if necessary needs, like light and temperature, are met. To accomplish this goal two types of sensor for each plant are used: a photoresistor and a TMP36 temperature sensor to be able to read the luminosity and the temperature room. In case of allarming values a led or a buzzer will be turned on to notify the owner, that will also have the possibility to view the data sent to the backend and to test the actuators via a website. The led and the buzzer are one for each plant.\
Depending on the values there are three states:
- Normal: plant's conditions are met.
- Allarming: the plant's conditions are not optimal but they don't require owner intervention. The led will be turned on
- Critical: the plant's conditions are critical and require the owner to take action. The buzzer will start beeping.\
The microcontroller read data from the sensors and sends it every five seconds.
## Components
### Hardware
The Nucleo-f401re board, a 32-bit ARM Cortex M4 based microcontroller.\
The TMP36 temperature sensor has an ±2°C accuracy over temperature and a 10 mV/°C scale factor and operates on 2.7-5.5 V. Can work on -40~+125°C.\
The photocell work on temperature has a working temperature of -25~+90 °C and can operates on 3-15 V.\
The led instead operates on -25~+85°C and operates with 5V.
### Software
RIOT, a small operating system for networked, memory-constrained systems with a focus on low-power wireless Internet of Things (IoT) devices. It is open-source software, released under the GNU Lesser General Public License (LGPL). 
Mosquitto, a message broker that implements several versions of the MQTT protocol, including the latest 5.0 revision. It is also a relatively lightweight software.\
Rsmb, a message broker that implements the MQTT-SN protocol, a protocol even lighter than MQTT.\
### Cloud
The following AWS service are used for the project:
* IoT-Core: for reading and publishing data to two topics 'localgateway_to_awsiot' and 'awsiot_to_localgateway'.
* DynamoDB: for storing data received from the sensor nodes
* S3 bucket: for hosting the dashboard that the client uses
* Gateway API: useful to offer an interface between aws services and the dashboard
* Lambda functions: allow the different services to comunicate


<img src="https://github.com/lorenzo1234881/IoT-IA1-2021/blob/main/images/schematic.png" width=800>


## Architecture
Each microcontroller reads values from the sensor and transmitts them using MQTT-SN to a gateway that will act both as MQTT-SN broker, MQTT broker and transparent bridge between the two. The messages from the sensor node to the cloud is sent to 'localgateway_to_awsiot'. Each microcontroller also subscribes to 'awsiot_to_localgateway' where aws will publish data telling the sensor node which actuator activate. The nucleo-f410re will send data to the gateway running rsmb and mosquitto that they act as MQTT-SN/transparent bridge and MQTT broker. Mosquitto then connects to the AWS IoT Core that will send the data that receives to a lambda function 'storeIntoDB', which send data to dyanmoDB and also publish data to 'awsiot_to_localgateway if 'light' and 'temperature' are lower than a fixed threshold.\
Meanwhile the dashboard fetches the data trhough the API offered by the API gateway, that will allow to: receive the last values red, receive the maximum, minimum and the average of the values received by aws in the last hour, and activate the actuators publishing on the 'awsiot_to_localgateway' topic. The lambda  'getValues.js' is responsable to fetch the data from the dynamoDB, 'controlActuators.js', instead is responsable to publish data on 'awsiot_to_localgateway'. 


<img src="https://github.com/lorenzo1234881/IoT-IA1-2021/blob/main/images/architecture.png" width=800>

## HowTo
* install RIOT OS, mosquitto, rsmb, awscli
* configure mosquitto to comunicate with aws
* access to the website: lorenzodiag2021.s3-website-us-east-1.amazonaws.com
* flash the firmware: 'make flash term'

In https://github.com/lorenzo1234881/IoT-IA1-2021/blob/main/how-it-was-done.txt there is a more detailed guide with useful links that helped in the creation of this project.

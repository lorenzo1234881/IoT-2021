# IoT-IA3-2021
## Pro and Cons of long-range low-power wide network

* Having more sensor nodes can help retrieve more data from different location and helps and how much the sensor values depend on the position, and because we are using LoRa we could have the LoRaWAN gateway far away from the sensor nodes (a realistic distance could be ~1 km)
* Having more nodes in a wireless sensor network makes manteinance more difficult and the system in total obviously less energy efficient. But is important to keep in mind that LoRa is designed to be energy efficient so it won't increase much the energy consumed per device.
* Using a wireless sensor network, the system as whole will be more tolerant against a sensor node faults. On the other end the crash of the LoRaWAN gateway, or the MQTT broker will cause the cloud backend to stop receiving data
* Because the network is low-energy, the network will have some serious limitations in throughput. Also with respect to low-power short-distance protocols, it is easy to notice an increase of latency to sending the packets
* An advantage of LoRaWAN with respect of other protocols is its security, that guarantee data integrity and confidentiality with the application server. For this purpose two keys are used: one to encrypt the payload, AppSKey, to provide end-to-end security, and one to provide integrity protection and encryption, NwkSKey. The keys are generated at each session 
* With edge computing we will have a less accurate analysis since the aggregate operations are performed directly on the board, however this allow to transmitts data after a longer period of time making LoRa more suitable for the project

## IoT Architecture
The B-L072Z-LRWAN1 nodes perform the edge analytics and send the aggregate data to The Things Network via LoRa. To connect TTN and AWS an A8 node running mosquitto is used. The node subscribe to a topic, where the data sent to the TTN is published. After receiveing data the node publish the data on a different topic that AWS IoT subscribes to and sends to Dynamo DB.
<img src="https://github.com/lorenzo1234881/IoT-IA3-2021/blob/master/images/architecture.png" width=800>

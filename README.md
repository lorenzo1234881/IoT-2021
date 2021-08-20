# IoT-IA2-2021
## Pro and Cons of Wireless Sensor Network
* Having more sensor nodes can help retrieve more data from different location and helps and how much the sensor values depend on the position
* Using a wireless sensor network, the system as whole will be more tolerant against a sensor node faults. On the other end the crash of a of the border router, or the MQTT broker will cause the cloud backend to stop receiving data
* Having more nodes in a wireless sensor network makes manteinance more difficult and the system in total obviously less energy efficient. But is important to keep in mind that 6LowPAN is designed to be energy efficient so it won't increase much the energy consumed per device.
* Because the network is low-energy, the network will have some serious limitations in throughput. This is not a big problem since we don't have an high frequency sampling, so the network won't congestionate.
## IoT Architecture
An M3 node is used as a border router while the ssh host propagete the ipv6 preifx to the nodes. An A8 nodes will run both the MQTT-S/MQTT broker that will receive the data published by m3 nodes running firmware the firware of this project
<img src="https://github.com/lorenzo1234881/IoT-IA2-2021/blob/main/images/architecture.png" width=800>
## Evaluation performances
* We can measure performances in terms of energy consumption (current, voltage, power) and network traffic (RSSI) thanks to the tools offered by IoT-LAB.
* Thanks to these monitoring tools we can see how much energy consumption will be affected increasing the number of nodes
* The RSSI, that is the indication of the radio power level received by the antenna of the node, shows us that if the RSSI is low, due to an high distance between the two nodes or maybe due to obstacle between them, affect negatively the throughput of the network causing retransmissions and delays 
### Node A8 Broker 
<img src="https://github.com/lorenzo1234881/IoT-IA2-2021/blob/main/images/a8_broker_power.png" width=500>
<img src="https://github.com/lorenzo1234881/IoT-IA2-2021/blob/main/images/a8_broker_radio.png" width=500>
### Node M3 Border Router
<img src="https://github.com/lorenzo1234881/IoT-IA2-2021/blob/main/images/m3_router_power.png" width=500>
<img src="https://github.com/lorenzo1234881/IoT-IA2-2021/blob/main/images/m3_router_radio.png" width=500>
### Node M3-1
<img src="https://github.com/lorenzo1234881/IoT-IA2-2021/blob/main/images/m3_node1_power.png" width=500>
<img src="https://github.com/lorenzo1234881/IoT-IA2-2021/blob/main/images/m3_node1_radio.png" width=500>
### Node M3-2
<img src="https://github.com/lorenzo1234881/IoT-IA2-2021/blob/main/images/m3_node2_power.png" width=500>
<img src="https://github.com/lorenzo1234881/IoT-IA2-2021/blob/main/images/m3_node2_radio.png" width=500>
* From the graphs it is possible to notice an increase in power when the firmware is flashed, and in the M3 nodes a regular change in power. These frequent increases correspond to the moments when the sensor nodes (m3-1, m3-2) retrieve the data from the sensors and send it to the border router, that will then send it to the A8 node.
* The moments when instead there's a decrease in power on the sensor nodes happen when the sensor node sleep and doesn't receive any packet from the cloud backend.
* For all the nodes the RSSI is always -91 dBm, with the exception of the starting noise in the m3-1 node. This is positive because it means that there is very little interference in sending the packets between nodes.

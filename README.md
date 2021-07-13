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
* We can measure performances in terms of energy consumption (current, voltage, power) and network traffic (rssi) thanks to the tools offered by IoT-LAB.
* Thanks to these monitoring tools we can see how much energy consumption will be affected increasing the number of nodes
* The rssi, that is the indication of the radio power level received by the antenna of the node, shows us that if the rssi is low, due to an high distance between the two nodes or maybe due to obstacle between them, affect negatively the throughput of the network causing retransmissions and delays 

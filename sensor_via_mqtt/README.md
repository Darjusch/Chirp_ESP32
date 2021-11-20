## ESP32 + Mosquitto MQTT Setup on DigitalOcean + NodeRed

# Setting up the project

## ESP Software Setup

### Clone the repository

### Instal the Arduino IDE - https://www.arduino.cc/en/software

### Set the Arduino IDE up to be able to work with ESP32 - https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html

### Install the following Requirments:

DHT Sensor Library - https://www.arduino.cc/reference/en/libraries/dht-sensor-library/

Adafruit Unified Sensor Library - https://www.arduino.cc/reference/en/libraries/adafruit-unified-sensor/

ESP async TCP Library - https://github.com/me-no-dev/ESPAsyncTCP

Async MQTT Client Library - https://github.com/marvinroger/async-mqtt-client

In the sensor_via_mqtt.ino file change the IP ADDRESS to the Server where you host your MQTT Broker if you don't use ours. 
// MQTT_HOST IPAddress(HOST_IP_ADDRESS)

Change the SSID and Password to the one you are using:
//WIFI_SSID YOURSSIDNAME
//WIFI_PASSWORD YOURWIFIPASSWORD

## ESP32 Hardware Setup 

DHTPIN 4     // Digital pin connected to the DHT sensor
DHTTYPE DHT22

RAIN_PIN 32 // Digital pin connected to Rain sensor

## On our DigitalOcean instance ( Request Permission if you want to change something / Access it )

First connect to wifi:
```
sudo nano /etc/wpa_supplicant/wpa_supplicant.conf
```
( Doesn't work with Code or Factory wifi ) - Needs 2.4 GHZ
Mobile hotspot always works 

There you can specify the network:
```
network={
    ssid="" // your network name
    psk="" // your network password
}
```

Then install Docker:

```
sudo apt-get update && sudo apt-get upgrade
```

```
curl -fsSL https://get.docker.com -o get-docker.sh
```

```
sudo sh get-docker.sh
```

Pull the latest MQTT Mosquitto image:

```
docker pull eclipse-mosquitto
```

Following this https://github.com/eclipse/mosquitto/issues/2040:

```
docker run -d -it -p 1883:1883 eclipse-mosquitto mosquitto -c /mosquitto-no-auth.conf
```

the -d lets it run in the background with 

```
docker ps -a
```
you can verify that the image is running





### NodeRed Setup

https://randomnerdtutorials.com/esp32-mqtt-publish-bme280-arduino/

IP Changes everytime ( We need a static IP ) so we have to change it in the nodered ui dashboard and in the esp32 code(#define MQTT_HOST IPAddress).
You can see the ip address with the command: 
ifconfig
the ip you need is written under wlan0
We have to install the Dashboard manually:


If the Port is below 2000 you have to allow the access to it.
sudo ufw allow 1880

https://randomnerdtutorials.com/getting-started-with-node-red-dashboard/

For some reason the Dashboard is not installed:

cd ~/.node-red
npm install node-red-dashboard

For changes to take affect you have to reboot:

sudo reboot

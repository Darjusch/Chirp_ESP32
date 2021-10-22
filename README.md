## Raspberry PI Mosquitto MQQT Setup + ESP 32 + NodeRed

### Raspberry PI Setup


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

Pull the latest MQQT Mosquitto image:

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



### ESP32 Setup


https://randomnerdtutorials.com/esp32-mqtt-publish-bme280-arduino/

The Raspberry PI Always changes IP be aware to change that in the ESP32 IP setting


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


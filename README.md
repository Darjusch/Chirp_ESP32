Raspberry PI Mosquitto MQQT Setup + ESP 32 + NodeRed



Raspberry PI Setup


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



ESP 32 Setup


https://randomnerdtutorials.com/esp32-mqtt-publish-bme280-arduino/

The Raspberry PI Always changes IP be aware to change that in the ESP 32 IP setting


NodeRed Setup


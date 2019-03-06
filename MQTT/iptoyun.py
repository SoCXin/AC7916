# import paho.mqtt.client as mqtt
import paho.mqtt.publish as publish
import time
import socket
import fcntl
import struct

HOST= "120.79.63.76"
PORT= 1883

SID= "/ssh/ip"
user = {'username':"qitas", 'password':"qitas"}


def get_ip_address(ifname):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    return socket.inet_ntoa(fcntl.ioctl(
        s.fileno(),
        0x8915,  # SIOCGIFADDR
        struct.pack('256s', bytes(ifname[:15], 'utf-8'))
    )[20:24])
hostname=socket.getfqdn(socket.gethostname())
hostip=get_ip_address('wlan0')

def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe(SID)

def on_message(client, userdata, msg):
    print(msg.topic+" "+msg.payload.decode("utf-8"))

def ip2yun(): 
    while True:
        client_id = time.strftime('%Y%m%d%H%M%S',time.localtime(time.time()))
        ipdata=hostname+"@"+hostip
        publish.single(SID, ipdata, qos = 1,hostname=HOST,port=PORT, client_id=client_id,auth =user)
        time.sleep(60)

if __name__ == '__main__':   
    ip2yun()


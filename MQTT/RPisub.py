# import paho.mqtt.client as mqtt
import paho.mqtt.publish as publish
import paho.mqtt.client as mqtt
import crc16
import serial
import time

HOST= "120.79.63.76"
PORT= 1883

SID= "/rpi"

# crc = crc16.crc16xmodem(b'1234')
# crc = crc16.crc16xmodem(b'56789', crc)
# print(crc)

# 打开串口
# ser = serial.Serial("/dev/ttyUSB0", 115200)

def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe(SID)

def on_message(client, userdata, msg):
    print(msg.topic+" "+msg.payload.decode("utf-8"))

if __name__ == '__main__':

    # client.publish("test", "你好 MQTT", qos=0, retain=False)  # 发布消息
    # ser.write("qitas mqtt publish \n".encode())
    # publish.single("qitas", "qitas MQTT", qos = 1,hostname=HOST,port=PORT, client_id=client_id,auth = {'username':"Qitas", 'password':"123456"})
    client_id = time.strftime('%Y%m%d%H%M%S',time.localtime(time.time()))
    print("client_id："+client_id)
    client = mqtt.Client(client_id)    # ClientId不能重复，所以使用当前时间	
    client.username_pw_set("qitas", "qitas")  # 必须设置，否则会返回「Connected with result code 4」
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect(HOST, PORT, 60)
    client.loop_forever()

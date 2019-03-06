#!/bin/bash
sudo apt update
sudo apt upgrade -y

#python3 install
sudo apt install python-dev python3-dev -y
sudo apt install python-pip python3-pip -y
sudo apt install libgtk2.0-dev -y
sudo apt install libatlas-base-dev gfortran -y

pip3 install numpy
pip3 install paho.mqtt


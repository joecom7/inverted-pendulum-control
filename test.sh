#! /usr/bin/bash

sleep 5
timeout 300s sudo /home/pi/inverted-pendulum-control/stabilizer
#timeout 10s sudo /home/pi/inverted-pendulum-control/meca500_ethercat_cpp/robot-test
mkdir ../prove/2407
cp pendulum.csv ../prove/2407/pendulum.csv

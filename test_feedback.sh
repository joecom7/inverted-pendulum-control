#! /usr/bin/bash

sleep 5
sudo /home/pi/inverted-pendulum-control/test-feedback
#timeout 10s sudo /home/pi/inverted-pendulum-control/meca500_ethercat_cpp/robot-test
mkdir ../prove/2009
cp feedback.csv ../prove/2009/feedvack.csv

#! /usr/bin/bash

sleep 5
sudo /home/pi/inverted-pendulum-control/test-controllo
#timeout 10s sudo /home/pi/inverted-pendulum-control/meca500_ethercat_cpp/robot-test
mkdir prove_definitive
cp test_controllo.csv prove_definitive

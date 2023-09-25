#! /usr/bin/bash

sleep 5
sudo /home/pi/inverted-pendulum-control/test-caduta-libera
#timeout 10s sudo /home/pi/inverted-pendulum-control/meca500_ethercat_cpp/robot-test
mkdir prove_definitive
cp caduta_libera.csv prove_definitive

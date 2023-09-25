#! /usr/bin/bash

sleep 5
sudo /home/pi/inverted-pendulum-control/test-onda-quadra
#timeout 10s sudo /home/pi/inverted-pendulum-control/meca500_ethercat_cpp/robot-test
mkdir prove_definitive
cp onda_quadra_nuova.csv prove_definitive

#! /usr/bin/bash

sleep 5
sudo /home/pi/inverted-pendulum-control/test-feedback-posa
#timeout 10s sudo /home/pi/inverted-pendulum-control/meca500_ethercat_cpp/robot-test
mkdir prove_definitive
cp feedback.csv prove_definitive
cp feedback_tempi.csv prove_definitive

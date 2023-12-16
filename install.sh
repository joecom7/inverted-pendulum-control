# enabling i2c since it's required
# to use the lcd display
# Raspi config non-interactive mode: 1 = false/off 0=true/on
sudo raspi-config nonint do_i2c 0
## installing the required packages
sudo apt-get update
sudo apt-get -y install build-essential git wget tar cmake libeigen3-dev
# installing pigpio
cd /tmp
wget https://github.com/joan2937/pigpio/archive/master.zip
unzip master.zip
cd pigpio-master
make
sudo make install
cd /home/$USER
#installing the program
git clone https://github.com/Vanvitelli-Robotics/inverted-pendulum-control
cd inverted-pendulum-control
git submodule update --init
cmake .
make
sudo cp stabilizer /usr/bin/stabilizer
sudo chmod +x /usr/bin/stabilizer
cat <<EOF | sudo tee /etc/systemd/system/stabilizer.service
[Unit]
Description=Inverted pendulum stabilizer service
After=network.target 
StartLimitIntervalSec=0

[Service]
Type=simple
Restart=always
RestartSec=1
ExecStart=/usr/bin/stabilizer

[Install]
WantedBy=multi-user.target
EOF
sudo systemctl enable stabilizer
sudo reboot
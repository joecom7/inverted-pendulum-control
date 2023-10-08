# inverted-pendulum-control

This project controls a robotic arm (Meca500) to stabilize an inverted pendulum.

## Requirements

To compile you need the pigpio library. Install with:

```bash
wget https://github.com/joan2937/pigpio/archive/master.zip
unzip master.zip
cd pigpio-master
make
sudo make install
```

## Compile

Compile with:

```bash
cmake .
make
```

## Run

Run with:

```bash
sudo ./stabilizer
```

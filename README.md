# inverted-pendulum-control

Progetto per la tesi di laurea di Giovanni Francesco Comune e Gianluca Pandolfi

## Requisiti

Per compilare il progetto sono necessario pigpio e libmatio-dev. Installarli con:
```
wget https://github.com/joan2937/pigpio/archive/master.zip
unzip master.zip
cd pigpio-master
make
sudo make install
sudo apt -y install libmatio-dev
```

Sono inoltre necessari python3 e la libreria cython. Installarla con:

```
pip3 install Cython
```



## Compilazione

Compilare con:

```
make
```

## Esecuzione
Eseguire con:

```
sudo ./stabilizer
```

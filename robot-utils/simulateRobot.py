import socket
import time
import os
import random
import math
from threading import Thread
 
def encode_ascii(str):
    return bytes(str,'ascii') + b'\x00'

eobCounter = 0

start_of_program = time.time()
 
 
def handleConnect(conn):
 
    conn.sendall(encode_ascii((str("[3000][Connected to Meca500 127.0.0.1]"))))
 
    timestamp = time.time() - start_of_program
    #temp = str("[2310][") + str(timestamp)
    #conn.sendall(encode_ascii((str(temp + str(", 0, 0]")))))
#
    temp = str("[2320][") + str(timestamp)
    conn.sendall(encode_ascii((str(temp + str(", 0, 0, 0, 0]")))))
#
    #temp = str("[2209][") + str(timestamp)
    #conn.sendall(encode_ascii((str(temp + str(", 0]")))))
#
    #temp = str("[2228][") + str(timestamp)
    #conn.sendall(encode_ascii((str(temp + str(", 0, 0, 0, 0, 0, 0]")))))
#
    #temp = str("[2229][") + str(timestamp)
    #conn.sendall(encode_ascii((str(temp + str(", 0, 0, 0, 0, 0, 0]")))))
 
 
 
 
def handleActivate(conn):
    print("activate")
    responseCode = str("[2000]") # [2000][Motors activated.]
    responseMessage = str("[Motors activated.]")
    response = encode_ascii((responseCode + responseMessage))
    print(response)
    time.sleep(2)
    conn.sendall(response)
 
def handleHome(conn): # [2002][Homing done.]
    responseCode = str("[2002]")
    responseMessage = str("[Homing done.]")
    time.sleep(1)
    conn.sendall(encode_ascii((responseCode + responseMessage)))
 
def handleDeactivate(conn): # [2004][Motors deactivated.]
    print("activate")
    responseCode = str("[2004]")
    responseMessage = str("[Motors deactivated.]")
    conn.sendall(encode_ascii((responseCode + responseMessage)))
 
def handleClearMotion(conn): # [2044][The motion was cleared.]
    responseCode = str("[2044]")
    responseMessage = str("[The motion was cleared.]")
    conn.sendall(encode_ascii((responseCode + responseMessage)))

def handleResetError(conn):
    responseCode = str("[2006]") # [2006][There was no error to reset.]
    responseMessage = str("[There was no error to reset.]")
    conn.sendall(encode_ascii((responseCode + responseMessage)))
    
def handleStatusRobot(conn):
    global eobCounter
    responseCode = str("[2007]") # [2006][There was no error to reset.]
    if eobCounter < 2:
        responseMessage = str("[1,1,1,0,0,0,0]")
    else :
        responseMessage = str("[1,1,1,0,0,1,1]")
    eobCounter += 1
    conn.sendall(encode_ascii((responseCode + responseMessage)))

def buildMessageVel(vel):
    messCode = str("[2214]") # [2214][t, ẋ , ẏ , ż, ωx, ωy, ωz]
    messPayload = str(f"[{time.time() - start_of_program}, {vel}, 0, 0, 0, 0, 0]")
    return encode_ascii(messCode + messPayload)

def buildMessagePose(pos):
    messCode = str("[2027]") # [2027][t, x , y , z, alpha, beta, gamma]
    messPayload = str(f"[{time.time() - start_of_program}, {pos}, 0, 0, 0, 0, 0]")
    return encode_ascii(messCode + messPayload)

 
 
 
 
def handleData(data, conn):
    #print(data)
    if data == "ActivateRobot\0":
        handleActivate(conn)
    elif data == "DeactivateRobot\0":
        handleDeactivate(conn)
    elif data == "Home\0":
        handleHome(conn)
    elif data == "ClearMotion\0":
        handleClearMotion(conn)
    elif data == "ResetError\0":
        handleResetError(conn)
    elif data == "GetStatusRobot\0":
        handleStatusRobot(conn)
    elif (data.find("MovePose") != -1) and (data.find('\0') != -1):
        print(data)
    #elif (data.find("MoveLin") != -1) and (data.find('\0') != -1):
    #    conn.sendall(encode_ascii(("Motion received")))
    else: 
        return

def feedbackLoop():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s2: 
        s2.bind((HOST, PORTMONITORING))
        s2.listen()
        conn2, addr2 = s2.accept()
        with conn2:
            print(f"Connected by {addr2} | monitoring side\n\n")
            while True:
                messageVel = buildMessageVel(random.randint(0, 100))
                messagePose = buildMessagePose(random.randint(0, 100))
                #print(message)
                conn2.sendall(messageVel)
                conn2.sendall(messagePose)
                time.sleep(0.004)
 
 
 
HOST = "0.0.0.0"  # Static IP of the robot
PORTCONTROL = 10000  # Port on which the robot listen and send response to user
PORTMONITORING = 10001 # Optional feedback messages port

sommaTempi = 0
iterazioni = 0
 
TIMEOUT = 20 # Timeout for connection waiting, seconds

thread = Thread(target=feedbackLoop)
thread.start()

#param = os.sched_param(os.sched_get_priority_max(os.SCHED_FIFO))
#os.sched_setscheduler(0, os.SCHED_FIFO, param)

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s: 
    s.bind((HOST, PORTCONTROL))
    s.listen()
    conn, addr = s.accept()
    with conn:
        print(f"Connected by {addr} | Control side\n\n") # Confirm of connection
        handleConnect(conn)
        lastMessage = time.time()
        while True:
            data = conn.recv(1024).decode("ascii") 
            if len(data) != 0:
                tempoPassato = time.time() - lastMessage
                lastMessage = tempoPassato + lastMessage
                sommaTempi += tempoPassato
                iterazioni += 1
                media = sommaTempi/(iterazioni)
                #print(f"media = {media*1000000}")
                handleData(data, conn)
            
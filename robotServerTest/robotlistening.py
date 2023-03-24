import socket
import time
 
def encode_ascii(str):
    return bytes(str,'ascii') + b'\x00'
 
 
def handleConnect(conn):
 
    conn.sendall(encode_ascii((str("[3000][Connected to Meca500 127.0.0.1]"))))
 
    timestamp = time.time() / 1000000
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
    responseMessage = str("[Motors activated.]\0")
    conn.sendall(encode_ascii((responseCode + responseMessage)))
 
def handleHome(conn): # [2002][Homing done.]
    responseCode = str("[2002]")
    responseMessage = str("[Homing done.]\0")
    conn.sendall(encode_ascii((responseCode + responseMessage)))
 
def handleDeactivate(conn): # [2004][Motors deactivated.]
    responseCode = str("[2004]")
    responseMessage = str("[Motors deactivated.]\0")
    conn.sendall(encode_ascii((responseCode + responseMessage)))
 
def handleClearMotion(conn): # [2044][The motion was cleared.]
    responseCode = str("[2044]")
    responseMessage = str("[The motion was cleared.]\0")
    conn.sendall(encode_ascii((responseCode + responseMessage)))

def handleResetError(conn):
    responseCode = str("[2006]") # [2006][There was no error to reset.]
    responseMessage = str("[There was no error to reset.]\0")
    conn.sendall(encode_ascii((responseCode + responseMessage)))

 
 
 
 
def handleData(data, conn):
    if data == "ActivateRobot\0":
        handleActivate(conn)
        print("Activation handled\n")
    elif data == "DeactivateRobot\0":
        handleDeactivate(conn)
        print("Deactivate handled\n")
    elif data == "Home\0":
        handleHome(conn)
        print("Home handled\n")
    elif data == "ClearMotion\0":
        handleClearMotion(conn)
        print("ClearMotion handled\n")
    elif data == "ResetError\0":
        handleResetError(conn)
        print("ResetError handled\n")
    else: 
        return
 
 
 
HOST = "localhost"  # Static IP of the robot
PORTCONTROL = 10000  # Port on which the robot listen and send response to user
PORTMONITORING = 10001 # Optional feedback messages port
 
TIMEOUT = 20 # Timeout for connection waiting, seconds
 
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s: 
    s.bind((HOST, PORTCONTROL))
    s.listen()
    conn, addr = s.accept()
    with conn:
        print(f"Connected by {addr}\n\n") # Confirm of connection
        handleConnect(conn)
        lastMessage = time.time()
        while True:
            # Checking if there are no more messages sent by client
            if time.time() - lastMessage > TIMEOUT:
                break
            data = conn.recv(1024).decode("ascii") 
            if len(data) != 0:
                print(data)
                handleData(data, conn)
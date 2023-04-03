import cython
from libcpp cimport bool
import os
from threading import Thread
import time

lastSpeed = 0
lastPosition = 0
MOVEMENT_AXIS = 0 # cambiare nel caso la velocità interessata non sia la x

def retrieveData():
    global robotFeedback
    global lastSpeed
    global lastPosition
    #param = os.sched_param(os.sched_get_priority_max(os.SCHED_OTHER))
    #os.sched_setscheduler(0, os.SCHED_OTHER, param)
    while True:
        print("sono nel thread")
        _, pose, speed, _ = robotFeedback.get_data(wait_for_new_messages=True)
        try:
            axisPosition = pose[MOVEMENT_AXIS]-1000
            axisSpeed = speed[1+MOVEMENT_AXIS]
        except TypeError:
            axisSpeed = lastSpeed
            axisPosition = lastPosition
        lastPosition = axisPosition
        lastSpeed = axisSpeed
        print(lastSpeed)
        print(lastPosition)
        #time.sleep(0.004)


cdef public void meca_init(bool bypass_robot, const char* robot_ip):
    param = os.sched_param(os.sched_get_priority_max(os.SCHED_FIFO))
    os.sched_setscheduler(0, os.SCHED_FIFO, param)
    global robotController
    global robotFeedback
    global BYPASS_ROBOT
    BYPASS_ROBOT = bypass_robot
    robot_ip_str = robot_ip.decode()
    if not bypass_robot:
        print(f"python : importo il modulo MecademicRobot")
        from mecademic_pydriver import RobotController
        from mecademic_pydriver import RobotFeedback
        print(f"python : creo un oggetto robot con ip {robot_ip_str}")
        robotController = RobotController(robot_ip_str)
        robotFeedback = RobotFeedback(robot_ip_str)

cdef public bool meca_connect():
    global robotController
    global robotFeedback
    if BYPASS_ROBOT:
        return True
    print("python : mi connetto al robot")

    try:
        robotController.connect()
        robotFeedback.connect()
    except Exception:
       print("python : la connessione al robot non e'  andata a buon fine")
       return False
    else:
       print("python : la connessione al robot e' andata a buon fine")
       thread = Thread(target=retrieveData)
       thread.start()
       return True

cdef public void meca_activate():
    global robotController
    print("python : attivo il robot")
    response = robotController.ActivateRobot()
    print(f"robot : {response}")

cdef public void meca_home():
    global robotController
    response = robotController.Home()
    print(f"robot : {response}")

cdef public void meca_deactivate():
    global robotController
    print("python : disattivo il robot")
    response = robotController.DeactivateRobot()
    print(f"robot : {response}")

cdef public void meca_disconnect():
    global robotController
    global robotFeedback
    print("python : mi disconnetto dal robot")
    robotController.disconnect()
    robotFeedback.disconnect()

cdef public void meca_reset_error():
    global robotController
    response = robotController.ResetError()
    print(f"robot : {response}")

cdef public void print_velocity(double n):
    print(f"Sono Python! Ho ricevuto il numero {n}")

cdef public double meca_get_velocity():
    return lastSpeed

cdef public double meca_get_position():
    return lastPosition

cdef public void meca_move_lin_vel_trf(double vel):
    global robotController
    if MOVEMENT_AXIS == 0:
        p_dot = (vel,0,0)
    elif MOVEMENT_AXIS == 1:
        p_dot = (0,vel,0)
    elif MOVEMENT_AXIS == 2:
        p_dot = (0,0,vel)
    w = (0,0,0)
    robotController.MoveLinVelTRF(p_dot,w)

cdef public void meca_set_conf(int c1, int c2, int c3):
    global robotController
    robotController.SetConf(c1,c2,c3)

cdef public void meca_move_pose(double x, double y, double z, double alpha, double beta, double gamma):
    global robotController
    position = (x,y,z)
    orientation = (alpha, beta, gamma)
    robotController.MovePose(position, orientation)

cdef public bool meca_block_ended():
    global robotController
    status = robotController.GetStatusRobot()
    if status.get("eob") == 1:
        return True
    else:
        return False

cdef public void meca_set_monitoring_interval(double monitoring_interval):
    global robotController
    robotController.SetRealTimeMonitoring(["2210","2211","2214"])
    robotController.SetMonitoringInterval(monitoring_interval)
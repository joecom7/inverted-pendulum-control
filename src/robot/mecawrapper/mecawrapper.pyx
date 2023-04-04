import cython
from libcpp cimport bool
import os
import threading
import time

lastSpeed = 0
lastPosition = 0
lastPositionTimestamp = 0
lastSpeedTimestamp = 0
MOVEMENT_AXIS = 0 # cambiare nel caso la velocità interessata non sia la x
program_ended = False
rt_monitoring_interval = 5e-3

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

cdef public bool meca_connect(bool activate_feedback):
    global ACTIVATE_FEEDBACK
    global robotController
    #global robotFeedback

    ACTIVATE_FEEDBACK = activate_feedback

    if BYPASS_ROBOT:
        return True
    print("python : mi connetto al robot")

    try:
        robotController.connect()
        #if activate_feedback:
        #    robotFeedback.connect()
    except Exception:
       print("python : la connessione al robot non e'  andata a buon fine")
       return False
    else:
       print("python : la connessione al robot e' andata a buon fine")
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

cdef public void meca_update_data():
    global robotFeedback
    global lastSpeed
    global lastPosition
    global lastSpeedTimestamp
    global lastPositionTimestamp
    _, pose, speed, _ = robotFeedback.get_data(wait_for_new_messages=True)
    try:
        axisSpeed = speed[1+MOVEMENT_AXIS]
        speedTimestamp = speed[0]
    except TypeError:
        axisSpeed = lastSpeed
        speedTimestamp = lastSpeedTimestamp
    try:
        axisPosition = pose[MOVEMENT_AXIS+1]
        positionTimestamp = pose[0]
    except TypeError:
        axisPosition = lastPosition
        positionTimestamp = lastPositionTimestamp
    lastSpeed = axisSpeed
    lastPosition = axisPosition
    lastPositionTimestamp = positionTimestamp
    lastSpeedTimestamp = speedTimestamp

cdef public double meca_get_velocity():
    global lastSpeed
    return lastSpeed

cdef public double meca_get_position():
    global lastPosition
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

cdef public void meca_end_program():
    global program_ended
    program_ended = True

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
    global rt_monitoring_interval
    global ACTIVATE_FEEDBACK
    robotController.SetRealTimeMonitoring(["2210","2214","2201"])
    robotController.SetMonitoringInterval(monitoring_interval)
    rt_monitoring_interval = monitoring_interval
    if ACTIVATE_FEEDBACK:
        thd = threading.Thread(target=update_loop)
        thd.start()

cdef public double meca_get_speed_timestamp():
    global lastSpeedTimestamp
    return lastSpeedTimestamp

cdef public double meca_get_position_timestamp():
    global lastPositionTimestamp
    return lastPositionTimestamp

def update_loop():
    param = os.sched_param(os.sched_get_priority_max(os.SCHED_FIFO) - 1)
    os.sched_setscheduler(0, os.SCHED_FIFO, param)
    
    global rt_monitoring_interval
    global program_ended
    global robotFeedback
    try:
        robotFeedback.connect()
    except Exception:
        print("python : la connessione al robot non e'  andata a buon fine")
    while not program_ended:
        meca_update_data()
        #time.sleep(rt_monitoring_interval)
        #time.sleep(rt_monitoring_interval)

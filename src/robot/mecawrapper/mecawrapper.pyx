import cython
from libcpp cimport bool
import os

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
       print("python : la connessione al robot non è andata a buon fine")
       return False
    else:
       print("python : la connessione al robot è andata a buon fine")
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
    global robotFeedback
    _, _, speed, _ = robotFeedback.get_data(wait_for_new_messages=False)
    return speed[1] # cambiare nel caso la velocità interessata non sia la x
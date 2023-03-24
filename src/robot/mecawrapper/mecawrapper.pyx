import cython
from libcpp cimport bool

robot_ip = 'localhost'

cdef public void meca_init(bool bypass_robot):
    global robot
    if not bypass_robot:
        print(f"python : creo un oggetto robot con ip {robot_ip}")
        import MecademicRobot
        robot = MecademicRobot.RobotController(robot_ip)

cdef public void meca_connect():
    global robot
    print("python : provo a connnettermi al robot")
    connection_successful = robot.connect()
    if not connection_successful:
        print("python : la connessione al robot non è andata a buon fine")

cdef public void meca_activate():
    global robot
    print("python : provo ad attivare il robot")
    robot.ActivateRobot()

cdef public void meca_home():
    global robot
    robot.home()

cdef public void meca_deactivate():
    global robot
    print("python : provo a disattivare il robot")
    robot.DeactivateRobot()

cdef public void meca_disconnect():
    global robot
    print("python : provo a disconnettermi dal robot")
    robot.disconnect()

cdef public void meca_reset_error():
    global robot
    robot.ResetError()

cdef public void print_velocity(float f):
    print(f"Sono Python! Ho ricevuto il numero {f}")
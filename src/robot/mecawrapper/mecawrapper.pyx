import cython
from libcpp cimport bool

cdef public void meca_init(bool bypass_robot, const char* robot_ip):
    global robot
    robot_ip_str = robot_ip.decode()
    if not bypass_robot:
        print(f"python : importo il modulo MecademicRobot")
        import MecademicRobot
        print(f"python : creo un oggetto robot con ip {robot_ip_str}")
        robot = MecademicRobot.RobotController(robot_ip_str)

cdef public void meca_connect():
    global robot
    print("python : mi connetto al robot")
    connection_successful = robot.connect()
    if not connection_successful:
        print("python : la connessione al robot non è andata a buon fine")
    else:
        print("python : la connessione al robot è andata a buon fine")

cdef public void meca_activate():
    global robot
    print("python : attivo il robot")
    response = robot.ActivateRobot()
    print(f"robot : {response}")

cdef public void meca_home():
    global robot
    response = robot.home()
    print(f"robot : {response}")

cdef public void meca_deactivate():
    global robot
    print("python : disattivo il robot")
    response = robot.DeactivateRobot()
    print(f"robot : {response}")

cdef public void meca_disconnect():
    global robot
    print("python : mi disconnetto dal robot")
    robot.disconnect()

cdef public void meca_reset_error():
    global robot
    response = robot.ResetError()
    print(f"robot : {response}")

cdef public void print_velocity(double n):
    print(f"Sono Python! Ho ricevuto il numero {n}")
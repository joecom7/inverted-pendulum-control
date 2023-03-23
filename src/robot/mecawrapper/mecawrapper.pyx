import cython
#import MecademicRobot

cdef public void meca_init():
    global robot
    #robot = MecademicRobot.RobotController('192.168.0.100')

cdef public void meca_connect():
    global robot
    robot.connect()

cdef public void meca_activate():
    global robot
    robot.ActivateRobot()

cdef public void meca_home():
    global robot
    robot.home()

cdef public void meca_deactivate():
    global robot
    robot.DeactivateRobot()

cdef public void meca_disconnect():
    global robot
    robot.disconnect()

cdef public void meca_reset_error():
    global robot
    robot.ResetError()

cdef public void print_velocity(float f):
    print(f"Sono Python! Ho ricevuto il numero {f}")
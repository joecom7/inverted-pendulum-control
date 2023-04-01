from mecademic_pydriver import RobotController
from mecademic_pydriver import RobotFeedback
robot_ip_str = "localhost"
robotController = RobotController(robot_ip_str)
robotFeedback = RobotFeedback(robot_ip_str)

try:
    robotController.connect()
    robotFeedback.connect()
except Exception:
   print("error")
else:
   print("no error")

joints, pose, speed, robot_status = robotFeedback.get_data()
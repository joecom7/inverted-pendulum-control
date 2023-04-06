#include "imports.hpp"
#include "CodeHandler.cpp"


class RobotController
{
private:
    CodeHandler *handler = NULL; // Handler for messages received
    int Robotsocket; // Socket via Robot
    struct sockaddr_in address; // Address and port for the robot
    bool connected_status;
public:
    RobotController(char* IP_address, int port);
    void init();
    void Connect();
    void disconnect();


};



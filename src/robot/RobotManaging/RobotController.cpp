#include "RobotController.hpp"

RobotController::RobotController(char* IP_address, int port)
{
    connected_status = false;
    try
    {
        Robotsocket = socket(AF_INET, SOCK_STREAM, 0);
        if(Robotsocket < 0)
            throw (Robotsocket);
    }
    catch(...)
    {
        printf("Socket failed");
    }
    
    handler = new CodeHandler(Robotsocket);

    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    if(inet_pton(address.sin_family, IP_address, &address.sin_addr) < 0)
    {
        printf("Invalid IP address\n");
    }
    init();
}

void RobotController::init()
{
    
}

void RobotController::Connect()
{
    if(!connected_status)
    {
        try
        {
            int status = connect(Robotsocket, (struct sockaddr *) & address, sizeof(address));
            if(status < 0)
                throw status;
        }
        catch(...)
        {
            perror("Connection failed");
        }

        connected_status = handler->ReceiveResponse(0);

        
    }
    else
    {
        printf("Still connected to the robot\n");
    }
    
}
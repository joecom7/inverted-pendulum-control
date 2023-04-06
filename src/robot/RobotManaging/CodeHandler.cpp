#include "CodeHandler.hpp"

const char* CodeHandler::codes[] = {"3000", "3001"};

CodeHandler::CodeHandler(int socket)
{
    if(socket > 0)
    {
        this -> handlerSocket = socket;
    }
    
	timeout_select.tv_sec = 0;
	timeout_select.tv_usec = 0;
	FD_ZERO(&reading_socket);
	FD_SET(handlerSocket, &reading_socket);

}

void CodeHandler::ReceiveResponse(int timeout = 0)
{
    if(!timeout)
    {
        timeout_select.tv_usec = timeout;
    }
    else
    {
        timeout_select.tv_usec = 0;
    }

    FD_ZERO(&reading_socket);
	FD_SET(handlerSocket, &reading_socket);

    int select_result = select(handlerSocket + 1, &reading_socket, NULL, NULL, &timeout_select);

    switch (select_result)
    {

    case 0:
        printf("Nothing to read\n");
        break;

    case 1:
        handleResponse();
        break;

    case -1:
        perror("select()");
        break;

    default:
        printf("Nothing happened on Select\n");
        break;

    }

}

void CodeHandler::handleResponse() // Non blocking
{
    char buffer[1024]; // has to be checked what can be maximum and minimum size of packet in receive
    recv(handlerSocket, buffer, sizeof(buffer), MSG_DONTWAIT); // Non blocking recv call
    // format messages in right form

}



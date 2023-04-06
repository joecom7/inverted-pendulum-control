#include "imports.hpp"

class CodeHandler
{
private:
    static const char* codes[]; // list of all codes handled
    int handlerSocket;
    struct timeval timeout_select; // timeout for waiting new messages
    fd_set reading_socket;
public:
    CodeHandler(int socket);
    void ReceiveResponse(int timeout); // timeout defined in microseconds
    void handleResponse(); // Non blocking
};
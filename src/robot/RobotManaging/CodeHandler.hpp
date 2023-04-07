#include "imports.hpp"

class CodeHandler
{
private:
    static const char* response_codes[]; // list of all codes handled
    static const char* error_codes[];
    int handlerSocket;
    struct timeval timeout_select; // timeout for waiting new messages
    fd_set reading_socket;
public:
    CodeHandler(int socket);
    bool ReceiveResponse(int timeout); // timeout defined in microseconds
    void handleResponse(); // Non blocking
};
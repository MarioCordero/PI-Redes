#ifndef Socket_h
#define Socket_h

#include "VSocket.h"

class Socket : public VSocket { // Socket Class, inherits from VSocket

    public:
        // Constructors
        // Default constructor
        Socket();
        Socket(char type, bool IPv6 = false);
        Socket(int id);
        ~Socket();

        // Socket connection methods
        int Connect(const char* host, int port);
        int Connect(const char* host, const char* service);
        int getSocketDescriptor() const;

        // Read and write methods
        size_t Read(void* buffer, size_t size);
        size_t Write(void* buffer, size_t size);
        size_t Write(char* message);

        // Accept method for server socket
        Socket* Accept();

    protected:
        // Add any private members for basic socket here, if needed.
};

#endif
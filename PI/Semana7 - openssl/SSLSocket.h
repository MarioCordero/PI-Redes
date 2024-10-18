#ifndef SSLSocket_h
#define SSLSocket_h

#include "Socket.h" // Include base class
#include <openssl/ssl.h>
#include <openssl/err.h>

class SSLSocket : public Socket { // SSLSocket Class, inherits from Socket

    public:
        // Constructor
        SSLSocket();
        // Constructor that forwards the type to the base class constructor
        SSLSocket(char type, bool IPv6 = false);
        ~SSLSocket();

        // Server-specific SSL methods
        void SSLInitServer(const char* certFile, const char* keyFile);
        void SSLCreate(Socket* serverSocket);
        void SSLAccept();

        // SSL read/write methods
        int SSLRead(char* buffer, int bufferSize);
        int SSLWrite(const char* message, int length);

        // SSL client certificates and cipher info
        void SSLShowCerts();
        const char* SSLGetCipher();

        // SSL initialization for clients
        void SSLInit(); 
        void SSLConnect(char* hostname, int port);
        void CopyContext(SSLSocket* server);

        // SSL certificate and key loading
        void LoadCertificates(const char* certFile, const char* keyFile);
        void SSLSetCredentials(const char* username, const char* password);

        // SSL cleanup
        void SSLCleanup();

    private:
        // SSL-related private members
        SSL* ssl;
        SSL_CTX* sslContext;
        const char* username;
        const char* password;
};

#endif
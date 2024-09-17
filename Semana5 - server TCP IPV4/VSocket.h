/**
 *  Universidad de Costa Rica
 *  ECCI
 *  CI0123 Proyecto integrador de redes y sistemas operativos
 *  2024-ii
 *  Grupos: 3 y 5
 *
 * (versión Fedora)
 *
 **/


#ifndef VSocket_h
#define VSocket_h
#include <string>
 
class VSocket {
	public:
		void CreateVSocket( char, bool=false );
		void CreateVSocket( int );
		~VSocket();

		void Close();
		int MakeConnection(const char *, int);
		int MakeConnection( const char *, const char *);
		int Listen( int queue ); 	//NEW
		int Bind( int );
		int DoAccept(); 			//NEW
		int Shutdown( int mode ); 	//NEW
		size_t sendTo( const void *, size_t, void * );
		size_t recvFrom( void *, size_t, void * );
		std::string processHttpResponse(const std::string& response);

		//Virtual methods
		virtual int Connect( const char *, int ) = 0;
		virtual int Connect( const char *, const char * ) = 0;
		virtual size_t Read( void *, size_t ) = 0;
		virtual size_t Write( const void *, size_t ) = 0;
		virtual size_t Write( const char * ) = 0;
		virtual VSocket * Accept() = 0; //NEW

	protected:
		int idSocket;	// Socket identifier
		bool IPv6;		// Is IPv6 socket?
		int port;		// Socket associated port
		
};

#endif // VSocket_h
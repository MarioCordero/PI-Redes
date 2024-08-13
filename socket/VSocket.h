/**
 *  Universidad de Costa Rica
 *  ECCI
 *  CI0123 Proyecto integrador de redes y sistemas operativos
 *  2024-iI
 *  Grupo: 3
 *
 * (versión Fedora)
 *
 **/


#ifndef VSocket_h
#define VSocket_h
 
class VSocket {
   public:
       void CreateVSocket( char, bool=false );
       void CreateVSocket( int );
      ~VSocket();

      void Close();
      int MakeConnection( const char *, int );
      virtual int Connect( const char *, int ) = 0;

      virtual size_t Read( const void *, size_t ) = 0;
      virtual size_t Write( const void *, size_t ) = 0;
      virtual size_t Write( const char * ) = 0;

   protected:
      int idSocket;   // Socket identifier
      bool IPv6;      // Is IPv6 socket?
      int port;       // Socket associated port
        
};

#endif // VSocket_h


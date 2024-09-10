# Semana4 - socket msg UDP IPv4 & IPv6

## Descripción
**Objetivo**

- Construir un certificado auto-firmado, haga capturas de pantalla de los pasos que lleva a cabo para presentar como evidencia de este trabajo en clase.

**Tareas**

- Instalar el paquete "easy-rsa" (dnf install easy-rsa)
- Iniciar PKI (/usr/share/easy-rsa/3.1.7/easyrsa init-pki)
- Crear la autoridad certificadora [CA] (/usr/share/easy-rsa/3.1.7/easyrsa build-ca [puede usar CI0123 CA para el nombre])
- Generar la llave de encripcion [DH] (/usr/share/easy-rsa/3.1.7/easyrsa gen-dh)
- Generar la llave para el servidor (/usr/share/easy-rsa/3.1.7/easyrsa build-server-full redes )
- Generar la llave para el cliente (/usr/share/easy-rsa/3.1.7/easyrsa build-client-full )
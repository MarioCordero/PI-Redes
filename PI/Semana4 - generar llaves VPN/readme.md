# Semana4 - socket msg UDP IPv4 & IPv6 Trabajo en clase

## Descripción
**Objetivo**

- Construir un certificado auto-firmado, haga capturas de pantalla de los pasos que lleva a cabo para presentar como evidencia de este trabajo en clase.

**Tareas**

- Construir un certificado auto-firmado, haga capturas de pantalla de los pasos que lleva a cabo para presentar como evidencia de este trabajo en clase

  - Instalar el paquete "easy-rsa" (dnf install easy-rsa)
  - Iniciar PKI (/usr/share/easy-rsa/3.1.7/easyrsa init-pki)
  - Crear la autoridad certificadora [CA] (/usr/share/easy-rsa/3.1.7/easyrsa build-ca [puede usar CI0123 CA para el nombre])
  - Generar la llave de encripcion [DH] (/usr/share/easy-rsa/3.1.7/easyrsa gen-dh)
  - Generar la llave para el servidor (/usr/share/easy-rsa/3.1.7/easyrsa build-server-full redes )
  - Generar la llave para el cliente (/usr/share/easy-rsa/3.1.7/easyrsa build-client-full )

- Construir un certificado auto-firmado para Apache

  - Investigar como agregar el certificado auto-firmado para Apache y hacer una documentación con el procedimiento aplicado en la computadora del laboratorio
  - Instalar Apache (dnf install httpd)
  - Instalar el certificado para que lo use Apache
  - Agregar al documento una prueba de que el Apache está usando el certificado generado

**Informe Linux Mint (Debian)**

Lo que hice para generar el certificado, ver más en PDF

Construir un certificado auto-firmado

-   ```bash
    #Install easy-rsa
    sudo apt install easy-rsa
    ```

-   ```bash
    #Init the PKI
    cd /usr/share/easy-rsa/
    sudo ./easyrsa init-pki
    ```

-   ```bash
    #Create a certification unit
    sudo ./easyrsa build-ca
    # I generate a passkey, then set the name [Easy-RSA CA]:CI0123 CA
    ```

-   ```bash
    #Generate an encript key
    sudo ./easyrsa gen-dh
    ```

-   ```bash
    #Generate a key for the server
    sudo ./easyrsa build-server-full redes
    ```

-   ```bash
    #Generate a key for the client
    sudo ./easyrsa build-client-full cliente-redes
    ```

Construir un certificado auto-firmado para Apache

-   ```bash
    #Installing Apache2
    sudo apt update
    sudo apt install apache2
    ```

-   ```bash
    #Start Apache2
    sudo systemctl start apache2
    ```

-   ```bash
    #Execute Apache2 when startup the PC
    sudo systemctl enable apache2
    ```

-   ```bash
    #Install openssl
    sudo apt install openssl
    ```

-   ```bash
    #Generate a private key
    sudo openssl genrsa -out /etc/ssl/private/certificado-redes.key 2048
    ```

-   ```bash
    #Generate auto-signed certificate, fill the info that request
    sudo openssl req -new -x509 -key /etc/ssl/private/certificado-redes.key -out /etc/ssl/certs/certificado-redes.crt -days 365
    ```

Configurar para que lo use Apache

-   ```bash
    #Open the .conf of Apache
    sudo nano /etc/apache2/sites-available/default-ssl.conf
    ```

-   ```bash
    #Open the .conf of Apache
    #Personal experience, make a backup of the .conf file, and save it, will be useful on the future
    sudo nano /etc/apache2/sites-available/default-ssl.conf
    ```

Cambié el archivo .conf:

-   ```bash
    #-----------------------------OLD CONF-------------------------
    #SSLCertificateFile     /etc/ssl/certs/ssl-cert-snakeoil.pem
    #SSLCertificateKeyFile /etc/ssl/private/ssl-cert-snakeoil.key

    #-----------------------------NEW CONF-------------------------
    SSLCertificateFile      /etc/ssl/certs/certificado-redes.crt
    SSLCertificateKeyFile /etc/ssl/private/certificado-redes.key
    ```

-   ```bash
    #Save the changes and enable the site
    sudo a2enmod ssl
    sudo a2ensite default-ssl.conf
    ```

-   ```bash
    #Restart Apache
    sudo systemctl restart apache2
    ```

**Informe Linux (Fedora)**


-   ```bash
    #Key to everything -> 2003
    ```
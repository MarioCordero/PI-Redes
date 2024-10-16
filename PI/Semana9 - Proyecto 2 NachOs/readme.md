# Semana9 - Proyecto 2 NachOS (INDIVIDUAL)

   - ## Trabajo en clase Semana #9
      * diningPH
         * ir a `PI-Redes/Semana9 - Proyecto 2 NachOs/NachOSx64/code/threads`

         * `make depends`

         * `make`

         * `./nachos`

            A partir de aquí hay que hacer que funcione

         * Se trabajó en el archivo `synch.cc`
            * `Lock::Acquire()`
            * `Lock::Release()`
            * `Lock::isHeldByCurrentThread()`
            * `Lock::Condition()`
            * `Lock::Wait()`
            * `Lock::Signal()`
            * `Lock::Broadcast()`
         * Por aparte, en el mismo archivo `synch.cc`, trabajé los métodos
            * `Lock::Acquire()`

   - ## Descripción 
      Queremos correr varios programas de usuario en NachOS, ya que la configuración original solo permite uno. Para entender como los programas de usuario son cargados en memoria vamos a revisar `addrspace`, el constructor es quien recibe el archivo ejecutable abierto, comprueba que sea ejecutable y lo carga en la memoria principal.

      Un programa de usuario se ejecuta cuando se inicia NachOS con la opción `-x ../test/halt`, en este caso se corre el programa de usuario `halt` por el método `StartProcess` en el archivo `progtest.cc` del directorio `userprog`; este programa debe ser leido del disco, colocado en la memoria de MIPS y crear el hilo de kernel correspondiente para ejecutarlo.

   - ## Objetivos
      * Entender el procedimiento para cargar un programa de usuario de NachOS a la memoria.

      * Cambiar el sistema de memoria de NachOS para permitir que varios procesos o hilos puedan correr.

      * Construir elementos para realizar la administración de memoria.

      * Entender la estructura interna de los programas de usuario, en especial el encabezado:
         * Header, Text, InitData.

      * Entender como los programas de usuario son leidos del disco.

      * Entender las estructuras de datos necesarias para la administración de procesos.

      * Representación interna de un hilo, clase Thread.

      * Representación interna de la memoria de un hilo de usuario, clase AddrSpace.

      * Mapeo de memoria lógica a memoria física, PageTable.
      
      * Completar algunos llamados al sistema relacionados con la ejecución de hilos o procesos.
         * Fork, Yield, Exec, Join, Exit.

      * Entender la administración de recursos por medio de una tabla (tabla de recursos abiertos).

      * Completar los elementos de sincronización de hilos.
      
      * Completar la funcionalidad de los sockets.

   - ## Funcionamiento

      En esta entrega, vamos a modificar la clases "AddrSpace" en los archivos "addrspace.h" y "addrspace.cc" para darle soporte a la "paginación" en donde los elementos de memoria son manipulados como páginas.

      Vamos a dividir la memoria de NachOS en 32 páginas de 128 bytes cada una (ambas constantes) y colocar las estructuras de datos necesarias para realizar la administración de memoria.

      También vamos a construir los llamados al sistema para la ejecución de hilos y procesos.

   - ## Tareas
      * Revisar la declaración de la memoria de la máquina MIPS emulado por NachOS (machine/machine.h).

      * Investigar como NachoOS carga los programas a memoria originalmente, para ello hay que revisar los archivos "addrspace.h" y "addrspace.cc" en el directorio "userprog".  El constructor es el encargado de tomar el programa de usuario y cargarlo en la memoria.

      * Comprender como se determina el tamaño del programa de usuario y la cantidad de páginas a utilizar.
         * ¿Cuántas páginas se le da a la pila del proceso?

      * Entender qué es una tabla de páginas y su uso dentro de NachOS.  Comprender el mapeo que se hace originalmente entre las páginas lógicas y las físicas. Revisar el funcionamiento de los bits de protección de la tabla de páginas.

      * Crear las estructuras necesarias para manipular la memoria de MIPS con paginación:
         * Debe tener una estructura para determinar cuáles páginas de la memoria están libre/ocupadas (BitMap).

      * Definir una nueva manera de entregar las páginas al programa de usuario utilizando paginación, diferente de la manera secuencial.

      * Comprobar que el programa de usuario puede ser cargado en distintas páginas de memoria, colocando algunas de las páginas de memoria como ocupadas antes de la ejecución

      * Construir un programa de usuario que demuestre que sus cambios son funcionales, en el directorio "test"

      * Completar los siguientes llamados al sistema:
         * `Fork`, `Yield`, `Exec`, `Join`, `Exit`

   - ## Para este proyecto debe completar
      * La carga de procesos en páginas a partir del archivo ejecutable (`addrspace.h` y `addrspace.cc`).

      * La administración de memoria para que el programa de usuario solo ocupe las páginas necesarias.
         * Buscar una página de memoria libre.
         * Asignar esa página al programa del usuario y marcarla como ocupada.
         * Agregar esta información a la tabla de páginas del proceso.

      * Un programa de usuario de comprobación de los cambios, por ejemplo `shell.c`, `fork2.c`.
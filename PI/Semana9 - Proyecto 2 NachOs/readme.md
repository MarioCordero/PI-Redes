# Semana9 - Proyecto 2 NachOS (INDIVIDUAL)

- ## Trabajo en clase Semana #9 Martes


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
- ## Trabajo en clase Semana #9 Viernes


  - ### Objetivos:

    - Lograr que varios procesos puedan compartir la memoria de NachOS por medio de paginación y correr concurrentemente
    - Modificar el constructor de "AddrSpace" para permitir el uso de mapas de bits (bitmaps)
    - Definir las estructuras de datos para que puedan ser actualizadas por hilos concurrentes

  * `AddrSpace`

    1. **¿Cómo es leído el encabezado del programa ejecutable NachOS?**

       - El encabezado es leído con el método `ReadAt` de la clase `OpenFile`. Este método se utiliza para leer el archivo ejecutable y almacenar el encabezado en la variable `noffH` de tipo `NoffHeader`.
         ```bash
         executable->ReadAt((char *)&noffH, sizeof(noffH), 0);
         ```

       Se lee a partir de la posición 0 del archivo y se guarda el tamaño de noffH (el tamaño del encabezado).
    2. **¿De qué tamaño es el encabezado?**

       - El tamaño del encabezado es el tamaño de la estructura `NoffHeader`. En este caso, se lee el tamaño completo de esta estructura con `sizeof(noffH)`.
    3. **¿Cuál es el número mágico?**

       - El número mágico es una constante llamada `NOFFMAGIC`, que identifica el formato del archivo ejecutable. Después de leer el encabezado, se compara este número mágico con `noffH.noffMagic` para verificar que el archivo es válido. Si no coincide, se realiza una conversión con `WordToHost` y se intercambian los bytes usando `SwapHeader`.
    4. **¿Cómo es establecido el tamaño del proceso?**

       - El tamaño del proceso es la suma de los tamaños de los segmentos de código, datos inicializados, datos no inicializados y el espacio de pila (`UserStackSize`):

         ```bash
         size = noffH.code.size + noffH.initData.size + noffH.uninitData.size + UserStackSize;
         ```
         Luego, este tamaño se redondea hacia arriba para ajustarse al tamaño de las páginas de memoria, y se multiplica por el tamaño de la página:

         ``bash numPages = divRoundUp(size, PageSize); size = numPages * PageSize; ``
    5. **¿Cuál es el tamaño del segmento de código (T)?**

       - El tamaño del segmento de código es `noffH.code.size`. Esta variable contiene el tamaño del código que se debe cargar en memoria.
    6. **¿Cuál es el tamaño del segmento de datos (D)?**

       - El tamaño del segmento de datos inicializados es `noffH.initData.size`. También se incluye el tamaño de los datos no inicializados en `noffH.uninitData.size.`
    7. **¿Cuál es el tamaño de la pila (S)?**

       - El tamaño de la pila es una constante definida como `UserStackSize`, que se suma al tamaño total del proceso.
    8. **¿Cuál es el tamaño de una página?**

       - El tamaño de una página está definido por la constante `PageSize`. Este valor usualmente es 128 bytes en NachOS.
    9. **¿Alguno de estos tamaños puede ser cero?**

       - Sí, es posible que los segmentos de datos no inicializados o el código no tengan tamaño (sean cero). El constructor verifica si el tamaño es mayor que cero antes de intentar cargar estos segmentos:
         ```bc++
         if (noffH.code.size > 0) { ... }
         if (noffH.initData.size > 0) { ... }
         ```
    10. **¿Cómo el programa es cargado (del disco) a memoria?**

        - El programa es cargado a memoria utilizando el método `ReadAt` de `OpenFile`, que copia los datos desde el archivo a la memoria principal en la dirección especificada. Esto se hace en dos etapas, primero para el código y luego para los datos inicializados.
          ```bash
          executable->ReadAt(&(machine->mainMemory[noffH.code.virtualAddr]), noffH.code.size, noffH.code.inFileAddr);
          executable->ReadAt(&(machine->mainMemory[noffH.initData.virtualAddr]), noffH.initData.size, noffH.initData.inFileAddr);
          ```
    11. **¿Cómo se realiza la lectura del disco?**

        - La lectura del disco se realiza mediante el método `ReadAt`, que toma como parámetros la dirección en memoria donde se copiarán los datos, la cantidad de bytes a leer, y la posición en el archivo desde donde se leerán los datos.
    12. **¿Cómo se determina el archivo de donde se realiza la lectura?**

        - El archivo es el que se pasa como argumento al constructor `AddrSpace`, representado por el puntero `OpenFile *executable`.
    13. **¿Cómo se indica la cantidad de bytes que se quiere leer?**

        - La cantidad de bytes a leer se especifica como segundo parámetro en el método `ReadAt`, como se ve en los llamados:
          ```bash
          executable->ReadAt(&(machine->mainMemory[noffH.code.virtualAddr]), noffH.code.size, noffH.code.inFileAddr);
          ```
    14. **¿Cómo hacer para leer del archivo solo una página (128 bytes)?**

        - Para leer solo una página, se debe especificar 128 bytes como la cantidad de bytes a leer. Se puede hacer algo como:
          ```bash
          executable->ReadAt(&(machine->mainMemory[direccion_memoria]), PageSize, posicion_archivo);
          ```
    15. **¿Cuál es la posición dentro del archivo de lectura?**

        - La posición de lectura en el archivo se especifica como el tercer parámetro en `ReadAt`. Para el segmento de código, esta posición es `noffH.code.inFileAddr`, y para los datos inicializados es `noffH.initData.inFileAddr`.
    16. **¿Cómo es colocada en la memoria la información leída del disco?**

        - Los datos leídos se colocan directamente en la memoria principal (`machine->mainMemory`), en las direcciones virtuales indicadas por `noffH.code.virtualAddr` y `noffH.initData.virtualAddr`.
    17. **¿Cómo se puede cambiar esta dirección?**

        - Se puede cambiar especificando una dirección diferente en `machine->mainMemory[direccion]`, donde `direccion` es la dirección virtual correspondiente.
    18. **Estrategia para colocar las páginas de 128 bytes en las posiciones de memoria correspondientes**

        - Para cada página del archivo ejecutable (128 bytes), se debe leer desde la posición adecuada en el archivo y colocarlo en el frame correspondiente en la memoria principal, tomando en cuenta la tabla de traducción de páginas (page table).
    19. **¿Cómo manejar las páginas cuyo tamaño es menor que 128 bytes?**

        - Si el tamaño es menor que 128 bytes, se debe leer solo el tamaño disponible y llenar el resto de la página con ceros (inicialización con `bzero`).
    20. **Estrategia para manejar páginas con información de segmentos distintos**

        - Se puede definir un mecanismo que reserve espacio para ambos segmentos y separe lógicamente el código de los datos dentro de una misma página. Esto implica modificar la tabla de páginas para que contenga la información correcta de cada segmento.
  * Clase `TranslationEntry`

    * La clase `TranslationEntry` es esencial para la gestión de la memoria en sistemas operativos, ya que define las entradas de la tabla de traducción que mapean páginas de memoria virtual a páginas de memoria física. Cada entrada contiene información clave, como el número de la página virtual (`virtualPage`), el número de la página física (`physicalPage`), y varios bits de control que indican la validez de la traducción, si la página es de solo lectura, y si ha sido utilizada o modificada.

      En el contexto de la clase `AddrSpace`, la `pageTable` se construye y llena durante la inicialización del espacio de direcciones de un proceso. En un mapeo directo simple, cada página virtual se asigna directamente a la misma página física. Esto facilita la gestión inicial, pero limita la capacidad para ejecutar múltiples procesos simultáneamente.

      La información contenida en las instancias de `TranslationEntry` se utiliza durante la traducción de direcciones en métodos como `Machine::Translate`, que consultan la tabla de páginas para determinar cómo acceder a la memoria física. Aunque este enfoque básico es efectivo, se pueden implementar estrategias más avanzadas para mejorar la multitarea y la eficiencia del uso de memoria en sistemas más complejos.
  * Estudie los métodos de la clase BitMap

    * **¿Cómo crear un mapa de bits que represente la memoria del procesador MIPS?**

      Para crear un mapa de bits que represente la memoria del procesador MIPS, primero necesitas conocer el tamaño de la memoria en términos de páginas, lo cual se puede obtener de `machine/machine.h`. Luego, puedes instanciar la clase `BitMap` con ese número de páginas.

      ```c++
      int numPages = /* tamaño de memoria en páginas */;
      BitMap *bitmap = new BitMap(numPages);
      ```
    * **¿Cómo indica este mapa de bits si la página está ocupada o libre?**

      Cada bit en el mapa de bits indica el estado de una página de memoria: un bit establecido (1) significa que la página está ocupada, mientras que un bit no establecido (0) indica que la página está libre. Puedes usar los métodos `Mark(int which)` para marcar una página como ocupada y `Clear(int which)` para liberarla.
    * **¿Cómo asegurar acceso exclusivo al mapa de bits en un entorno de múltiples hilos?**

      Para garantizar acceso exclusivo al `BitMap`, puedes utilizar un mutex. Al acceder a los métodos del `BitMap`, el hilo debe bloquear el mutex antes de realizar la operación y liberarlo después. Esto evita condiciones de carrera y asegura que solo un hilo acceda a la estructura a la vez.

      ```c++
        	class SafeBitMap {
        		private:
        			BitMap *bitmap;
        			Mutex *mutex;

        		public:
        			SafeBitMap(int nitems) {
        				bitmap = new BitMap(nitems);
        				mutex = new Mutex();
        			}

        			void Mark(int which) {
        				mutex->Lock();
        				bitmap->Mark(which);
        				mutex->Unlock();
        			}

        			void Clear(int which) {
        				mutex->Lock();
        				bitmap->Clear(which);
        				mutex->Unlock();
        			}

        			bool Test(int which) {
        				mutex->Lock();
        				bool result = bitmap->Test(which);
        				mutex->Unlock();
        				return result;
        			}
        	};
      ```
  * Administración de la memoria de NachOS para programas de usuario

    * Cree una estructura de datos (BitMap) para representar las páginas de la memoria. Puede colocarla como una variable global en los archivos "system.h" y "system.cc" en el directorio "threads", su declaración debe ser similar a la de la variable "currentThread"

      * Se añadió a `system.h`  el bit map.
    * Además, esta nueva variable debe existir en el archivo "system.cc"

      * Se añadió a `system.cc` el bitmap como global
    * Inicialice la variable en el método "Initialize"

      ```c++
      void Initialize(int argc, char **argv)
      {
          int argCount;
          const char* debugArgs = "";
          bool randomYield = false;

          // 2007, Jose Miguel Santos Espino
          bool preemptiveScheduling = false;
          long long timeSlice;

      #ifdef USER_PROGRAM
          bool debugUserProg = false;  // paso a paso del programa de usuario
          MiMapa = new BitMap(NUM_PAGES);  // Inicializa el mapa de bits con el número de páginas disponibles en memoria
      #endif

      #ifdef FILESYS_NEEDED
          bool format = false;  // formatear disco
      #endif

          // Resto del código...
      }
      ```

    *  Haga que NachOS utilice esta estructura para solicitar las páginas de memoria y actualizar la tabla de páginas del proceso. Por el momento, asegúrese de que el programa obtenga las mismas páginas que antes (0, 1, 2, etc.). No cambie la manera en que el programa es cargado en la memoria
        * 
        ```console
        c22306@ECCI104L408200:~/Escritorio/PI-Redes/PI/Semana9 - Proyecto 2 NachOs/NachOSx64/code/userprog$ ./nachos

        No threads ready or runnable, and no pending interrupts.
        Assuming the program completed.
        Machine halting!

        Ticks: total 10, idle 0, system 10, user 0
        Disk I/O: reads 0, writes 0
        Console I/O: reads 0, writes 0
        Paging: faults 0
        Network I/O: packets received 0, sent 0

        Cleaning up...
        ```
    *  Recuerde asignar las páginas de los datos no inicializados y la pila 
        * Ok
    *  Verifique que el programa de usuario "halt" funcione correctamente 
        * Si, dice "Machine halting!"


  * Carga del programa en la memoria
    * Explique la manera en que el programa es cargado a la memoria
      * El proceso de carga de un programa en NachOS se realiza principalmente en el constructor de la clase `AddrSpace`. Este proceso incluye los siguientes pasos:

        1. Lectura del Encabezado: Se utiliza el método `ReadAt` para leer el encabezado del archivo ejecutable (formato NOFF), que contiene información sobre el tamaño de las secciones de código y datos.

        2. Cálculo del Tamaño Total: Se calcula el tamaño total de la memoria que necesita el programa, sumando el tamaño del código, datos iniciales, datos no inicializados y espacio para la pila.

        3. Asignación de Páginas: Se determina cuántas páginas de memoria son necesarias y se verifica que no excedan el número de páginas físicas disponibles en el sistema.

        4. Inicialización de la Tabla de Páginas: Se crea una tabla de traducción que asigna páginas virtuales a páginas físicas. Cada entrada en la tabla incluye información sobre la validez y la ubicación de la página.

        5. Inicialización de Memoria: La memoria se inicializa en cero para limpiar los segmentos de datos no inicializados y la pila.

        6. Copia de Secciones: Se leen las secciones de código y datos desde el archivo y se colocan en las direcciones de memoria correspondientes.

    * Explique qué hace el método `ReadAt` de la clase `OpenFile`
      * El método `ReadAt` de la clase `OpenFile` se utiliza para leer un número específico de bytes desde un archivo abierto. Su función principal es permitir que otros componentes del sistema accedan a los datos almacenados en archivos de manera controlada.

    * Explique la funcionalidad de los parámetros del método `ReadAt`
      * El método tiene la siguiente firma:
        ```c++
        int ReadAt(char *buffer, int size, int position);
        ```
      * `char *buffer`: Un puntero a un área de memoria donde se almacenarán los datos leídos del archivo. Este buffer debe tener suficiente espacio para contener el número de bytes especificados.

      * `int size`: La cantidad de bytes que se desea leer del archivo. Este valor indica cuánto del archivo se copiará en el buffer.

      * `int position`: La posición en el archivo desde donde comenzará la lectura. Permite la lectura en cualquier parte del archivo, lo que es útil para cargar secciones específicas del programa.

    * Indique porqué hay dos llamados a este método
      * En el constructor `AddrSpace::AddrSpace`, hay dos llamados a `ReadAt` porque un programa típico en formato NOFF tiene al menos dos secciones clave que deben ser cargadas en memoria:

        1. **Lectura del Segmento de Código**: El primer llamado a ReadAt se utiliza para leer el segmento de código del programa, que se encuentra en la sección correspondiente del archivo NOFF. Esto se copia en la memoria principal en la dirección virtual especificada.

        2. **Lectura del Segmento de Datos Inicial**: El segundo llamado a ReadAt se utiliza para leer el segmento de datos inicial. Este segmento contiene datos que el programa necesita para comenzar a ejecutarse y se copia en la memoria en la dirección correspondiente.

  *  Haga un diagrama que muestre la estructura de los programas de NachOS
      * En la arquitectura MIPS, el formato NOFF para los programas de usuario de NachOS se puede visualizar de la siguiente manera:
        ```lua
          |-------------------------------|
          | Encabezado (H)                |
          |-------------------------------|
          | Texto (TX)                    |
          |-------------------------------|
          | Datos Inicializados (DI)      |
          |-------------------------------|
          | Datos No Inicializados (DNI)  |
          |-------------------------------|
          | Pila                          |
          |-------------------------------|
        ```
      * Estructura del encabezado de un programa de usuario de NachOS
        * El encabezado del programa NOFF contiene información clave, como:
          * noffMagic: Un número mágico que identifica el formato NOFF.

          * tamaño de código: Indica el tamaño del segmento de texto.

          * tamaño de datos inicializados: Indica el tamaño del segmento de datos inicializados.

          * tamaño de datos no inicializados: Indica el tamaño del segmento de datos no inicializados.

      * Lectura del programa por páginas (128 bytes)
        * Páginas de 128 bytes: La memoria se gestiona en páginas de 128 bytes (o 1 página en el sistema).

          * **Lectura del segmento de texto (código):**

            Se leerá el segmento de texto en ciclos de 128 bytes hasta que se haya copiado todo el código en memoria.

            Si el tamaño del segmento no es un múltiplo exacto de 128 bytes, se debe leer la última página parcial con la cantidad de bytes restantes.

          * **Lectura del segmento de datos inicializados:**

            Se utiliza un enfoque similar al del código. Se lee el segmento de datos inicializados en páginas de 128 bytes hasta que se haya completado.

            Nuevamente, se manejará cualquier página parcial al final.

      * Reservas para datos no inicializados y pila
        * Datos No Inicializados: Aunque no se leen del disco, se deben reservar páginas en la memoria para el segmento de datos no inicializados.

        * Pila: Se reserva una o más páginas para la pila del proceso.

      * Colocación de páginas en la memoria
        * Las páginas de código y datos leídas se colocan en la posición de memoria especificada por la tabla de páginas del proceso. La tabla de páginas traduce las direcciones virtuales a direcciones físicas en la memoria.

      * Verificación del programa de usuario "halt"
        * Para verificar que el programa "halt" funcione correctamente:

          * Se debe compilar y ejecutar el programa "halt" en el entorno de NachOS.

          * Asegúrate de que se ejecute sin errores y que termine adecuadamente, liberando todos los recursos asignados.

      * Recursos a deshacerse al finalizar el programa
        
          * Cuando un programa finaliza (llamando a Exit), es necesario:

            **Liberar la memoria:** Destruir la tabla de páginas y liberar las páginas asignadas en la memoria.

            **Cerrar archivos:** Cerrar cualquier archivo abierto para evitar pérdidas de datos y liberar recursos.

            **Destruir objetos:** Liberar cualquier objeto dinámico que se haya creado durante la ejecución del programa.

    * Pruebas
      * Haga que algunas páginas (ejemplo 0,2,4,6,8,10) dentro de su administrador aparezcan como ocupadas antes de cargar cualquier programa en memoria, ésto se puede realizar dentro de constructor de "AddrSpace"
      *  Verifique el programa "halt" funcione 
      *  Compruebe que el programa "addrspacetest" funcione bien 
      *  Ejemplo de salida de "addrspacetest", requiere que "Exit" y "Write a la salida estándar estén implantados: 
        ```bash
          [farroyo@cloud userprog]$ ./nachos -x ../test/addrspacetest
          abcdefghijklmnopqrstuvwxyz{abcdefghijklmnopqrstuvwxyz{abcdefghijklmnopqrstuvwxyz{abcdefghijklmnopqrstuvwxyz{
          abcdefghijklmnopqrstuvwxyz{abcdefghijklmnopqrstuvwxyz{abcdefghijklmnopqrstuvwxyz{abcdefghijklmnopqrstuvwxyz{
          abcdefghijklmnopqrstuvwxyz{abcdefghijklmnopqrstuvwxyz{abcdefghijklmnopqrstuvwxyz{abcdefghijklmnopqrstuvwxyz{
          abcdefghijklmnopqrstuvwxyz{abcdefghijklmnopqrstuvwxyz{abcdefghijklmnopqrstuvwxyz{abcdefghijklmnopqrstuvwxyz{
          abcdefghijklmnopqrstuvwxyz{abcdefghijklmnopqrstuvwxyz{abcdefghijklmnopqrstuvwxyz{abcdefghijklmnopqrstuvwxyz{
          abcdefghijklmnopqrstuvwxyz{abcdefghijklmnopqrstuvwxyz{abcdefghijklmnopqrstuvwxyz{abcdefghijklmnopqrstuvwxyz{
          abcdefghijklmnopqrstuvwxyz{abcdefghijklmnopqrstuvwxyz{abcdefghijklmnopqrstuvwxyz{abcdefghijklmnopqrstuvwxyz{
          abcdefghijklmnopqrstuvwxyz{abcdefghijklmnopqrstuvwxyz{abcdefghijklmnopqrstuvwxyz{abcdefghijklmnopqrstuvwxyz{
          abcdefghijklmnopqrstuvwxyz{abcdefghijklmnopqrstuvwxyz{abcdefghijklmnopqrstuvwxyz{abcdefghijklmnopqrstuvwxyz{
          abcdefghijklmnopqrstuvwxyz{abcdefghijklmnopqrstuvwxy
          No threads ready or runnable, and no pending interrupts.
          Assuming the program completed.
          Machine halting!

          Ticks: total 31332, idle 0, system 560, user 30772
          Disk I/O: reads 0, writes 0
          Console I/O: reads 0, writes 1
          Paging: faults 0
          Network I/O: packets received 0, sent 0

          Cleaning up...
        ```
      *  Libere las páginas del primer punto
      * Corra de nuevo el programa "addrspacetest"
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

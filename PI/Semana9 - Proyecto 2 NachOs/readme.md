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

                  ```bash
                  numPages = divRoundUp(size, PageSize);
                  size = numPages * PageSize;
                  ```
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
               ```bash
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
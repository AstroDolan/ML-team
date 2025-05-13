#include <stdio.h>      // Para funciones de entrada/salida estándar (printf)
#include <stdlib.h>     // Para funciones generales (exit, malloc, etc.)
#include <unistd.h>     // Para la función sleep (pausas)
#include <pthread.h>    // Para la API de hilos POSIX

#define TRUE 1
#define FALSE 0
#define TAMANO_BUFFER 5         // Capacidad máxima del buffer compartido
#define NUM_PRODUCTORES 2       // Número de hilos productores
#define NUM_CONSUMIDORES 2      // Número de hilos consumidores
#define ITEMS_POR_PRODUCTOR 3 // Cuántos ítems producirá cada productor

// Buffer compartido y su estado
int buffer[TAMANO_BUFFER];      // El array que actúa como buffer
int cantidad_en_buffer = 0;   // Contador de ítems actualmente en el buffer

// Mecanismos de sincronización
pthread_mutex_t mutex_buffer = PTHREAD_MUTEX_INITIALIZER;   // Mutex para proteger el acceso al 'buffer' y 'cantidad_en_buffer'
pthread_mutex_t mutex_impresion = PTHREAD_MUTEX_INITIALIZER; // Mutex para sincronizar las salidas por 'printf' y evitar mensajes mezclados
pthread_cond_t cond_lleno = PTHREAD_COND_INITIALIZER;     // Variable de condición para que los productores esperen si el buffer está lleno
pthread_cond_t cond_vacio = PTHREAD_COND_INITIALIZER;     // Variable de condición para que los consumidores esperen si el buffer está vacío

// Contadores para la lógica de terminación
int total_items_a_producir = NUM_PRODUCTORES * ITEMS_POR_PRODUCTOR; // Cálculo del total de ítems que se generarán
int items_producidos_total = 0;   // Contador global de ítems producidos
int items_consumidos_total = 0; // Contador global de ítems consumidos

/*Explicación:

    Bibliotecas: Se incluyen las cabeceras necesarias para la E/S, utilidades estándar, sleep y, lo más importante, pthreads.
    Constantes: Se definen constantes para la configuración del problema: tamaño del buffer, número de productores/consumidores e ítems que cada productor generará. Esto facilita la modificación de estos parámetros.
    Buffer Compartido:
        buffer: Es un array de enteros que almacenará los ítems.
        cantidad_en_buffer: Indica cuántos espacios del buffer están actualmente ocupados.
    Sincronización:
        mutex_buffer: Es crucial. Antes de que cualquier hilo (productor o consumidor) intente modificar buffer o cantidad_en_buffer, debe adquirir este mutex. Esto asegura que solo un hilo a la vez pueda acceder a estas variables compartidas, previniendo corrupción de datos.
        mutex_impresion: Aunque no es estrictamente necesario para la lógica del productor-consumidor en sí, es una buena práctica para que los mensajes impresos por printf desde diferentes hilos no se intercalen de forma caótica, facilitando la depuración y el seguimiento.
        cond_lleno: Los productores la usarán para esperar si intentan añadir un ítem a un buffer que ya está lleno.
        cond_vacio: Los consumidores la usarán para esperar si intentan tomar un ítem de un buffer que está vacío.
    Contadores Globales:
        total_items_a_producir: Se calcula al inicio para saber cuándo todos los ítems esperados han sido generados.
        items_producidos_total: Rastrea cuántos ítems se han añadido al buffer en total por todos los productores.
        items_consumidos_total: Rastrea cuántos ítems se han retirado del buffer en total por todos los consumidores. Estos dos últimos son importantes para la condición de terminación de los consumidores.*/

void *funcion_productor(void *arg) {
    int id_productor = *((int *)arg); // Obtiene el ID del productor pasado como argumento

    for (int i = 0; i < ITEMS_POR_PRODUCTOR; ++i) { // Cada productor genera un número fijo de ítems
        sleep(1); // Simula el tiempo que toma producir un ítem
        int item_producido = id_productor * 100 + i; // Crea un ítem único (ej: Prod 1 -> 100, 101, 102; Prod 2 -> 200, 201, 202)

        pthread_mutex_lock(&mutex_buffer); // Adquiere el mutex para acceder/modificar el buffer

        // Bucle de espera si el buffer está lleno
        while (cantidad_en_buffer == TAMANO_BUFFER) {
            // Imprime mensaje (protegido por mutex_impresion)
            pthread_mutex_lock(&mutex_impresion);
            printf("[Productor %d] Buffer lleno (%d/%d). Esperando.\n", id_productor, cantidad_en_buffer, TAMANO_BUFFER);
            pthread_mutex_unlock(&mutex_impresion);

            // Espera en la variable de condición 'cond_lleno'.
            // 'pthread_cond_wait' libera 'mutex_buffer' automáticamente mientras espera,
            // y lo vuelve a adquirir antes de retornar.
            pthread_cond_wait(&cond_lleno, &mutex_buffer);

            // Imprime mensaje al despertar (protegido por mutex_impresion)
            pthread_mutex_lock(&mutex_impresion);
            printf("[Productor %d] Despertado de espera (lleno).\n", id_productor);
            pthread_mutex_unlock(&mutex_impresion);
        }

        // Sección crítica: Añade el ítem al buffer
        buffer[cantidad_en_buffer++] = item_producido; // Coloca el ítem y luego incrementa el contador
        items_producidos_total++;                      // Incrementa el contador global de ítems producidos

        // Imprime mensaje de producción (protegido por mutex_impresion)
        pthread_mutex_lock(&mutex_impresion);
        printf("[Productor %d] -> Produjo: %d. (Buffer: %d/%d)\n", id_productor, item_producido, cantidad_en_buffer, TAMANO_BUFFER);
        pthread_mutex_unlock(&mutex_impresion);

        // Señala a un posible consumidor que esté esperando porque el buffer estaba vacío
        pthread_cond_signal(&cond_vacio);

        pthread_mutex_unlock(&mutex_buffer); // Libera el mutex del buffer
    }

    // Imprime mensaje de finalización del productor (protegido por mutex_impresion)
    pthread_mutex_lock(&mutex_impresion);
    printf("[Productor %d] Termino de producir sus %d items.\n", id_productor, ITEMS_POR_PRODUCTOR);
    pthread_mutex_unlock(&mutex_impresion);

    pthread_exit(NULL); // Termina el hilo productor
}

/*Explicación del Productor:

    Identificación y Bucle: Cada hilo productor recibe un ID único. Entra en un bucle para producir ITEMS_POR_PRODUCTOR ítems.
    Simulación de Producción: sleep(1) simula el tiempo que lleva crear un ítem. El item_producido se genera de forma que sea fácilmente identificable.
    Bloqueo del Buffer (pthread_mutex_lock(&mutex_buffer)): Antes de intentar añadir al buffer, el productor bloquea mutex_buffer. Esto es esencial.
    Verificación de Buffer Lleno:
        Usa un while (no un if) para verificar si cantidad_en_buffer == TAMANO_BUFFER. El while es importante porque después de despertar de pthread_cond_wait, la condición podría haber cambiado de nuevo debido a otros hilos (despertar espurio o múltiples productores).
        Si está lleno, imprime un mensaje (usando mutex_impresion para evitar salidas mezcladas).
        pthread_cond_wait(&cond_lleno, &mutex_buffer): Esta es la clave. El productor se bloquea y espera en cond_lleno. Crucialmente, pthread_cond_wait libera mutex_buffer mientras el hilo está esperando. Esto permite que otros hilos (consumidores) puedan adquirir mutex_buffer y vaciar el buffer. Cuando cond_lleno es señalada por un consumidor y el productor despierta, pthread_cond_wait automáticamente re-adquiere mutex_buffer antes de continuar.
    Añadir Ítem al Buffer: Si el buffer no está (o ya no está) lleno, el productor añade item_producido al array buffer en la posición cantidad_en_buffer e incrementa cantidad_en_buffer. También incrementa items_producidos_total.
    Impresión y Señalización: Imprime que ha producido un ítem. Luego, llama a pthread_cond_signal(&cond_vacio). Esto despierta a un hilo consumidor que podría estar esperando en cond_vacio (porque el buffer estaba vacío). Si no hay consumidores esperando, la señal se pierde (no se acumula).
    Desbloqueo del Buffer (pthread_mutex_unlock(&mutex_buffer)): El productor libera mutex_buffer, permitiendo que otros hilos accedan a él.
    Finalización: Después de producir todos sus ítems, imprime un mensaje de finalización y termina con pthread_exit(NULL).*/

void *funcion_consumidor(void *arg) {
    int id_consumidor = *((int *)arg); // Obtiene el ID del consumidor

    while (TRUE) { // Bucle infinito, la condición de salida está dentro
        pthread_mutex_lock(&mutex_buffer); // Adquiere el mutex para acceder/modificar el buffer

        // Bucle de espera si el buffer está vacío Y no se han consumido todos los ítems producidos
        while (cantidad_en_buffer == 0 && items_consumidos_total < total_items_a_producir) {
            // Imprime mensaje (protegido por mutex_impresion)
            pthread_mutex_lock(&mutex_impresion);
            printf("[Consumidor %d] Buffer vacio (%d/%d). Esperando.\n", id_consumidor, cantidad_en_buffer, TAMANO_BUFFER);
            pthread_mutex_unlock(&mutex_impresion);

            // Espera en la variable de condición 'cond_vacio'.
            // Libera 'mutex_buffer' mientras espera y lo readquiere al despertar.
            pthread_cond_wait(&cond_vacio, &mutex_buffer);

            // Imprime mensaje al despertar (protegido por mutex_impresion)
            pthread_mutex_lock(&mutex_impresion);
            printf("[Consumidor %d] Despertado de espera (vacio).\n", id_consumidor);
            pthread_mutex_unlock(&mutex_impresion);
        }

        // Condición de terminación para el consumidor:
        // Si todos los ítems esperados ya fueron consumidos globalmente
        // Y el buffer está actualmente vacío (no queda nada por procesar).
        if (items_consumidos_total >= total_items_a_producir && cantidad_en_buffer == 0) {
            pthread_mutex_unlock(&mutex_buffer); // Libera el mutex antes de salir del bucle
            break; // Sale del bucle while(TRUE) y termina el hilo
        }

        // Sección crítica: Retira el ítem del buffer
        int item_consumido = buffer[0]; // Toma el primer ítem (FIFO)
        // Desplaza los ítems restantes en el buffer hacia el inicio
        for (int i = 1; i < cantidad_en_buffer; ++i) {
            buffer[i - 1] = buffer[i];
        }
        cantidad_en_buffer--;         // Decrementa el contador de ítems en el buffer
        items_consumidos_total++;     // Incrementa el contador global de ítems consumidos

        // Imprime mensaje de consumo (protegido por mutex_impresion)
        pthread_mutex_lock(&mutex_impresion);
        printf("[Consumidor %d] <- Consumio: %d. (Buffer: %d/%d)\n", id_consumidor, item_consumido, cantidad_en_buffer, TAMANO_BUFFER);
        pthread_mutex_unlock(&mutex_impresion);

        // Señala a un posible productor que esté esperando porque el buffer estaba lleno
        pthread_cond_signal(&cond_lleno);

        pthread_mutex_unlock(&mutex_buffer); // Libera el mutex del buffer

        sleep(1); // Simula el tiempo que toma consumir/procesar un ítem
    }

    pthread_exit(NULL); // Termina el hilo consumidor
}

/*Explicación del Consumidor:

    Identificación y Bucle Principal: Cada consumidor obtiene un ID. Entra en un bucle while(TRUE). La lógica de salida del bucle está dentro.
    Bloqueo del Buffer: Similar al productor, bloquea mutex_buffer.
    Verificación de Buffer Vacío y Condición de Continuación:
        Usa un while para verificar si cantidad_en_buffer == 0. Además, se asegura de seguir esperando solo si items_consumidos_total < total_items_a_producir. Esto evita que el consumidor se quede esperando indefinidamente si todos los ítems ya fueron producidos y consumidos.
        Si el buffer está vacío (y aún hay ítems por consumir en el sistema), imprime un mensaje y llama a pthread_cond_wait(&cond_vacio, &mutex_buffer). Similar al productor, esto libera mutex_buffer y espera una señal en cond_vacio (que un productor enviará).
    Condición de Terminación del Consumidor:
        Después del bucle de espera (o si no tuvo que esperar), verifica if (items_consumidos_total >= total_items_a_producir && cantidad_en_buffer == 0).
        Si se han consumido todos los ítems que se iban a producir en total, y el buffer está efectivamente vacío, el consumidor libera mutex_buffer y sale del bucle while(TRUE) usando break, lo que lo lleva a pthread_exit(NULL).
    Retirar Ítem del Buffer (Implementación FIFO simple):
        Si hay ítems y no es tiempo de terminar, toma el ítem de buffer[0].
        Luego, desplaza todos los elementos restantes del buffer una posición hacia la izquierda para llenar el hueco. Esto es una forma simple de implementar una cola, aunque puede ser ineficiente para buffers muy grandes (una lista enlazada o un buffer circular serían más eficientes).
        Decrementa cantidad_en_buffer e incrementa items_consumidos_total.
    Impresión y Señalización: Imprime que ha consumido un ítem. Llama a pthread_cond_signal(&cond_lleno). Esto despierta a un hilo productor que podría estar esperando en cond_lleno (porque el buffer estaba lleno).
    Desbloqueo del Buffer: Libera mutex_buffer.
    Simulación de Consumo: sleep(1) simula el tiempo de procesamiento del ítem consumido.
    Finalización: Al salir del bucle while(TRUE), el hilo termina con pthread_exit(NULL).*/

void *hilo_maestro(void *arg) {
    pthread_t hilos_productores[NUM_PRODUCTORES];
    pthread_t hilos_consumidores[NUM_CONSUMIDORES];

    int ids_productores[NUM_PRODUCTORES];
    int ids_consumidores[NUM_CONSUMIDORES];

    // Mensajes iniciales (protegidos por mutex_impresion)
    pthread_mutex_lock(&mutex_impresion);
    printf("[Hilo Maestro] Hilo maestro iniciado.\n");
    printf("[Hilo Maestro] Configuracion: Tamano del buffer=%d, Productores=%d, Consumidores=%d, Productos=%d, Total a producir: %d.\n", TAMANO_BUFFER, NUM_PRODUCTORES, NUM_CONSUMIDORES, ITEMS_POR_PRODUCTOR, total_items_a_producir);
    pthread_mutex_unlock(&mutex_impresion);

    // --- Creación de Hilos Productores ---
    pthread_mutex_lock(&mutex_impresion);
    printf("[Hilo Maestro] Creando hilos productores\n");
    pthread_mutex_unlock(&mutex_impresion);
    for (int i = 0; i < NUM_PRODUCTORES; ++i) {
        ids_productores[i] = i + 1; // Asigna un ID (1, 2, ...)
        // Crea el hilo productor, pasando su ID como argumento
        if (pthread_create(&hilos_productores[i], NULL, funcion_productor, &ids_productores[i]) != 0) {
            perror("[Hilo Maestro] Error al crear hilo productor");
            return (void*)1; // Indica error
        }
        pthread_mutex_lock(&mutex_impresion);
        printf("[Hilo Maestro] Creado Productor %d.\n", ids_productores[i]);
        pthread_mutex_unlock(&mutex_impresion);
    }

    // --- Creación de Hilos Consumidores ---
    pthread_mutex_lock(&mutex_impresion);
    printf("[Hilo Maestro] Creando hilos consumidores\n");
    pthread_mutex_unlock(&mutex_impresion);
    for (int i = 0; i < NUM_CONSUMIDORES; ++i) {
        ids_consumidores[i] = i + 1; // Asigna un ID (1, 2, ...)
        // Crea el hilo consumidor, pasando su ID como argumento
        if (pthread_create(&hilos_consumidores[i], NULL, funcion_consumidor, &ids_consumidores[i]) != 0) {
            perror("[Hilo Maestro] Error al crear hilo consumidor");
            return (void*)1; // Indica error
        }
        pthread_mutex_lock(&mutex_impresion);
        printf("[Hilo Maestro] Creado Consumidor %d.\n", ids_consumidores[i]);
        pthread_mutex_unlock(&mutex_impresion);
    }

    // --- Espera a que los Productores Terminen su Producción ---
    pthread_mutex_lock(&mutex_impresion);
    printf("[Hilo Maestro] Esperando a que los productores terminen su funcion.\n");
    pthread_mutex_unlock(&mutex_impresion);
    for (int i = 0; i < NUM_PRODUCTORES; ++i) {
        pthread_join(hilos_productores[i], NULL); // Espera a que el hilo productor i-ésimo termine
        pthread_mutex_lock(&mutex_impresion);
        printf("[Hilo Maestro] Productor %d ha finalizado.\n", ids_productores[i]);
        pthread_mutex_unlock(&mutex_impresion);
    }
    pthread_mutex_lock(&mutex_impresion);
    printf("[Hilo Maestro] Todos los productores han terminado su ciclo de produccion.\n");
    pthread_mutex_unlock(&mutex_impresion);

    // --- Asegurar que los Consumidores Despierten y Terminen ---
    // Este bucle es para asegurar que los consumidores, si se quedaron dormidos
    // justo cuando el buffer se vació pero aún no se cumplía la condición global de
    // 'items_consumidos_total >= total_items_a_producir', sean despertados.
    pthread_mutex_lock(&mutex_impresion);
    printf("[Hilo Maestro] Esperando a que todos los items sean consumidos.\n");
    pthread_mutex_unlock(&mutex_impresion);

    while (TRUE) {
        pthread_mutex_lock(&mutex_buffer); // Protege el acceso a 'items_consumidos_total'
        if (items_consumidos_total >= total_items_a_producir) {
            pthread_mutex_unlock(&mutex_buffer);
            // Una vez que todos los items han sido consumidos, envía una última señal
            // a todos los consumidores para que puedan verificar su condición de salida y terminar.
            // Esto es útil si algún consumidor se quedó esperando en cond_vacio justo
            // antes de que el último ítem fuera consumido por otro consumidor.
            pthread_cond_broadcast(&cond_vacio); // Despierta a TODOS los que esperan en cond_vacio
            break; // Salir del bucle del hilo maestro
        }
        // Si aún no se han consumido todos, despierta a los consumidores
        // por si alguno está esperando y los productores ya terminaron.
        pthread_cond_broadcast(&cond_vacio);
        pthread_mutex_unlock(&mutex_buffer);
        sleep(1); // Espera un poco antes de volver a verificar
    }

    pthread_mutex_lock(&mutex_impresion);
    printf("[Hilo Maestro] Todos los %d items has sido consumidos.\n", total_items_a_producir);
    pthread_mutex_unlock(&mutex_impresion);

    // --- Espera a que los Hilos Consumidores Terminen Completamente ---
    pthread_mutex_lock(&mutex_impresion);
    printf("[Hilo Maestro] Esperando a que las funciones de consumidor terminen.\n");
    pthread_mutex_unlock(&mutex_impresion);
    for (int i = 0; i < NUM_CONSUMIDORES; ++i) {
        pthread_join(hilos_consumidores[i], NULL); // Espera a que el hilo consumidor i-ésimo termine
        pthread_mutex_lock(&mutex_impresion);
        printf("[Hilo Maestro] Consumidor %d ha finalizado su funcion.\n", ids_consumidores[i]);
        pthread_mutex_unlock(&mutex_impresion);
    }
    pthread_mutex_lock(&mutex_impresion);
    printf("[Hilo Maestro] Todos los consumidores han terminado.\n");
    pthread_mutex_unlock(&mutex_impresion);

    // Mensaje final del hilo maestro
    pthread_mutex_lock(&mutex_impresion);
    printf("[Hilo Maestro] Hilo maestro terminando.\n");
    pthread_mutex_unlock(&mutex_impresion);

    pthread_exit(NULL); // Termina el hilo maestro
}

/*Explicación del Hilo Maestro:

    Inicialización: Declara arrays para almacenar los identificadores de los hilos (pthread_t) y sus IDs numéricos. Imprime mensajes de configuración inicial.
    Creación de Productores: Itera NUM_PRODUCTORES veces:
        Asigna un ID numérico al productor.
        Usa pthread_create para lanzar un nuevo hilo que ejecutará funcion_productor, pasándole su ID.
        Realiza una verificación básica de errores en pthread_create.
    Creación de Consumidores: Similarmente, crea NUM_CONSUMIDORES hilos que ejecutarán funcion_consumidor.
    Espera a Productores (pthread_join): El hilo maestro espera a que todos los hilos productores terminen su ejecución. pthread_join bloquea al hilo maestro hasta que el hilo productor especificado haya finalizado (haya llamado a pthread_exit o retornado). Esto asegura que todos los ítems que se iban a producir realmente se hayan intentado producir antes de proceder.
    Asegurar Finalización de Consumidores:
        Una vez que todos los productores han terminado, el total_items_a_producir ya no cambiará (a menos que hubiera errores, pero la lógica asume que los productores completan su cuota).
        El bucle while (TRUE) con items_consumidos_total >= total_items_a_producir se encarga de lo siguiente:
            Los consumidores podrían haberse quedado dormidos en cond_vacio si el buffer se vació justo antes de que el último lote de ítems fuera producido, o si un consumidor tomó el último ítem y otro se durmió.
            pthread_cond_broadcast(&cond_vacio): Despierta a todos los hilos que estén esperando en cond_vacio. Esto es una medida para asegurar que los consumidores tengan la oportunidad de revaluar su condición de salida (items_consumidos_total >= total_items_a_producir && cantidad_en_buffer == 0).
            Se usa mutex_buffer para leer items_consumidos_total de forma segura.
            El sleep(1) es para no consumir CPU excesivamente en este bucle de espera del maestro.
        Una vez que se confirma que todos los ítems han sido consumidos, se hace un último broadcast y se sale del bucle.
    Espera a Consumidores (pthread_join): Después de confirmar que todos los ítems fueron consumidos (y los consumidores han sido notificados para que puedan salir de sus bucles), el hilo maestro usa pthread_join para esperar a que cada hilo consumidor termine formalmente.
    Finalización: Imprime un mensaje y termina con pthread_exit(NULL).*/


int main() {
    pthread_t hilomaestro; // Identificador para el hilo maestro

    printf("--- Inicio del programa ---\n");

    // Crea el hilo maestro que orquestará a los productores y consumidores
    if (pthread_create(&hilomaestro, NULL, hilo_maestro, NULL) != 0) {
        perror("Error al crear hilo maestro");
        return 1; // Termina el programa con error
    }
    printf("Hilo maestro creado.\n");

    // Espera a que el hilo maestro termine toda su lógica
    pthread_join(hilomaestro, NULL);

    // Destruye los mutex y variables de condición una vez que ya no se necesitan
    // (después de que todos los hilos que los usan hayan terminado)
    pthread_mutex_destroy(&mutex_buffer);
    pthread_mutex_destroy(&mutex_impresion);
    pthread_cond_destroy(&cond_lleno);
    pthread_cond_destroy(&cond_vacio);

    printf("Fin del programa\n");

    return 0; // Termina el programa exitosamente
}

/*Explicación de main:

    Inicio: Imprime un mensaje de inicio.
    Crear Hilo Maestro: Crea un único hilo, el hilomaestro, que es el encargado de crear y gestionar a los productores y consumidores.
    Esperar al Hilo Maestro (pthread_join): El hilo principal (main) espera a que hilomaestro termine. Dado que hilomaestro a su vez espera a todos los productores y consumidores, cuando hilomaestro termina, significa que toda la simulación ha concluido.
    Limpieza de Recursos:
        pthread_mutex_destroy(): Libera los recursos asociados a los mutex.
        pthread_cond_destroy(): Libera los recursos asociados a las variables de condición.
        Es importante hacer esto solo después de que ningún hilo vaya a usar estos objetos de sincronización nuevamente.
    Fin: Imprime un mensaje de fin y retorna 0.

Flujo de Trabajo General:

    main inicia y crea el hilo_maestro.
    hilo_maestro imprime la configuración e inicia la creación de NUM_PRODUCTORES hilos productores y NUM_CONSUMIDORES hilos consumidores.
    Productores:
        Intentan producir ítems.
        Si el buffer está lleno, esperan en cond_lleno (liberando mutex_buffer).
        Si hay espacio, añaden un ítem, incrementan contadores y señalan cond_vacio (para despertar a un consumidor).
        Repiten hasta que han producido ITEMS_POR_PRODUCTOR ítems, luego terminan.
    Consumidores:
        Intentan consumir ítems.
        Si el buffer está vacío Y aún quedan ítems por ser consumidos en total, esperan en cond_vacio (liberando mutex_buffer).
        Si hay ítems, o si ya no quedan ítems por consumir y el buffer está vacío, verifican la condición de salida.
        Si no es tiempo de salir y hay ítems, toman un ítem, actualizan contadores y señalan cond_lleno (para despertar a un productor).
        Repiten hasta que la condición de salida (items_consumidos_total >= total_items_a_producir && cantidad_en_buffer == 0) se cumple, luego terminan.
    hilo_maestro primero espera (pthread_join) a que todos los productores terminen. Esto asegura que total_items_a_producir ítems han sido (o intentado ser) puestos en el sistema.
    Luego, hilo_maestro entra en un bucle para asegurar que todos los ítems producidos sean consumidos, usando pthread_cond_broadcast(&cond_vacio) para despertar a los consumidores que pudieran estar dormidos, hasta que items_consumidos_total alcanza total_items_a_producir.
    Finalmente, hilo_maestro espera (pthread_join) a que todos los consumidores terminen.
    Una vez que hilo_maestro termina, main recupera el control.
    main destruye los mutex y las variables de condición.
    El programa termina.

Puntos Clave de Sincronización:

    mutex_buffer es el guardián del buffer y cantidad_en_buffer. Siempre se bloquea antes de leer/escribir estas variables y se desbloquea después.
    pthread_cond_wait se usa siempre dentro de un bucle while y después de haber bloqueado el mutex asociado. Libera el mutex mientras espera y lo readquiere al despertar.
    pthread_cond_signal despierta a un hilo que espera. pthread_cond_broadcast despierta a todos los hilos que esperan en esa condición. La elección depende de la lógica (en este caso, signal es suficiente para productor->consumidor y viceversa cuando se añade/quita un solo ítem, pero broadcast se usa en el maestro para asegurar que todos los consumidores "revisen" si deben terminar).
    mutex_impresion solo sirve para que los printf no se solapen, mejorando la legibilidad de la salida.*/

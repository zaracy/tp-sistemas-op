#include "comunicacion.h"
#include "kernel.h"
#include <commons/log.h>
#include <unistd.h>

// Variables globales definidas en kernel.c
extern t_log* logger;
extern char* puerto_interrupt;

// Función principal del hilo que atiende conexiones entrantes por el puerto Interrupt.
// Este servidor escucha CPUs que envían señales de interrupción al Kernel.
void* atender_interrupt(void* args) {
    // Crear socket de escucha en el puerto configurado para Interrupt
    int socket_servidor = iniciar_servidor(puerto_interrupt);
    log_info(logger, "Servidor Interrupt escuchando en puerto %s", puerto_interrupt);

    while (1) {
        // Esperar que una CPU se conecte al puerto Interrupt
        int socket_cliente = esperar_cliente(socket_servidor);
        log_info(logger, "Se conectó una CPU al puerto Interrupt");

        // TODO: recibir interrupciones desde CPU (por ejemplo, una interrupción por fin de quantum)
        // recv(...) / send(...)
    }

    return NULL;
}


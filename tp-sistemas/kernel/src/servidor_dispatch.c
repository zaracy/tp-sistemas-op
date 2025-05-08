#include "comunicacion.h"
#include "kernel.h"
#include <commons/log.h>
#include <unistd.h>

// Variables globales definidas en kernel.c
extern t_log* logger;
extern char* puerto_dispatch;

// Función principal del hilo que atiende conexiones entrantes por el puerto Dispatch.
// Este servidor se queda escuchando conexiones de CPU que quieran enviar/recibir instrucciones.
void* atender_dispatch(void* args) {
    // Crear socket de escucha en el puerto configurado para Dispatch
    int socket_servidor = iniciar_servidor(puerto_dispatch);
    log_info(logger, "Servidor Dispatch escuchando en puerto %s", puerto_dispatch);

    while (1) {
        // Esperar que una CPU se conecte al puerto Dispatch
        int socket_cliente = esperar_cliente(socket_servidor);
        log_info(logger, "Se conectó una CPU al puerto Dispatch");

        // TODO: recibir y procesar instrucciones del CPU conectado
        // recv(...) / send(...)
    }

    return NULL;
}

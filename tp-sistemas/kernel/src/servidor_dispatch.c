#include "comunicacion.h"
#include "kernel.h"
#include <commons/log.h>
#include <unistd.h>

extern t_log* logger;
extern char* puerto_dispatch;

void* atender_dispatch(void* args) {
    int socket_servidor = iniciar_servidor(puerto_dispatch);
    log_info(logger, "Servidor Dispatch escuchando en puerto %s", puerto_dispatch);

    while (1) {
        int socket_cliente = esperar_cliente(socket_servidor);
        log_info(logger, "Se conectó una CPU al puerto Dispatch");

        // TODO: recibir instrucciones del CPU
        // recv(...) / send(...)
    }

    return NULL;
}

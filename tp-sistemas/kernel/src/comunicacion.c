#include "comunicacion.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// Crea y configura un socket de escucha (servidor) en el puerto especificado.
// Devuelve el descriptor del socket ya en estado de listen(), o -1 si falla.
int iniciar_servidor(char* puerto) {
    int socket_servidor;
    struct sockaddr_in direccion_servidor;

    // Crear el socket
    socket_servidor = socket(AF_INET, SOCK_STREAM, 0);

    // Configurar la dirección: IPv4, cualquier IP local, puerto especificado
    direccion_servidor.sin_family = AF_INET;
    direccion_servidor.sin_addr.s_addr = INADDR_ANY;
    direccion_servidor.sin_port = htons(atoi(puerto));

    // Enlazar el socket con la dirección configurada
    bind(socket_servidor, (struct sockaddr*)&direccion_servidor, sizeof(direccion_servidor));

    // Poner el socket en modo escucha
    listen(socket_servidor, SOMAXCONN);

    return socket_servidor;
}

// Espera una conexión entrante y la acepta.
// Devuelve un nuevo descriptor de socket correspondiente al cliente conectado.
int esperar_cliente(int socket_servidor) {
    struct sockaddr_in direccion_cliente;
    socklen_t tam_direccion = sizeof(struct sockaddr_in);

    // Aceptar una nueva conexión y devolver el socket del cliente
    return accept(socket_servidor, (struct sockaddr*)&direccion_cliente, &tam_direccion);
}

#include "comunicacion.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int iniciar_servidor(char* puerto) {
    int socket_servidor;
    struct sockaddr_in direccion_servidor;

    socket_servidor = socket(AF_INET, SOCK_STREAM, 0);
    direccion_servidor.sin_family = AF_INET;
    direccion_servidor.sin_addr.s_addr = INADDR_ANY;
    direccion_servidor.sin_port = htons(atoi(puerto));

    bind(socket_servidor, (struct sockaddr*)&direccion_servidor, sizeof(direccion_servidor));
    listen(socket_servidor, SOMAXCONN);

    return socket_servidor;
}

int esperar_cliente(int socket_servidor) {
    struct sockaddr_in direccion_cliente;
    socklen_t tam_direccion = sizeof(struct sockaddr_in);
    return accept(socket_servidor, (struct sockaddr*)&direccion_cliente, &tam_direccion);
}

#ifndef COMUNICACION_H
#define COMUNICACION_H

// Comunicación con CPU
void* atender_dispatch(void* args);
void* atender_interrupt(void* args);

// Funciones comunes de servidor
int iniciar_servidor(char* puerto);
int esperar_cliente(int socket_servidor);

#endif // COMUNICACION_H

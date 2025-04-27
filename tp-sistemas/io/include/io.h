#ifndef IO_H
#define IO_H

#include <commons/log.h>
#include <commons/config.h>
#include <stdbool.h>

typedef struct {
    t_log* logger;
    t_config* config;
    int kernel_fd;
    char* nombre_dispositivo;
} t_io;

void iniciar_io(char* config_path, char* nombre);
void finalizar_io(t_io* io);

#endif // IO_H
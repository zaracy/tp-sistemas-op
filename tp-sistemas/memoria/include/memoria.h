#ifndef MEMORIA_H
#define MEMORIA_H

#include <commons/log.h>
#include <commons/config.h>
#include <stdbool.h>

typedef struct {
    t_log* logger;
    t_config* config;
    int server_fd;
} t_memoria;

void iniciar_memoria(char* config_path);
void finalizar_memoria(t_memoria* memoria);

#endif // MEMORIA_H
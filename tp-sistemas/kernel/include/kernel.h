#ifndef KERNEL_H
#define KERNEL_H

#include <commons/log.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <stdbool.h>

typedef struct {
    t_log* logger;
    t_config* config;
    int server_fd_dispatch;
    int server_fd_interrupt;
    int server_fd_io;
} t_kernel;

void iniciar_kernel(char* config_path);
void finalizar_kernel(t_kernel* kernel);

#endif // KERNEL_H
#ifndef CPU_H
#define CPU_H

#include <commons/log.h>
#include <commons/config.h>
#include <stdbool.h>

typedef struct {
    t_log* logger;
    t_config* config;
    int dispatch_fd;
    int interrupt_fd;
} t_cpu;

void iniciar_cpu(char* config_path, char* cpu_id);
void finalizar_cpu(t_cpu* cpu);

#endif // CPU_H
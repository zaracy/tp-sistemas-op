#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <commons/config.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

t_log* logger;
t_config* config;
char* ip_kernel;
char* puerto_dispatch;
char* puerto_interrupt;
char* cpu_id;

// Funciones auxiliares
int conectar_a_kernel(char* ip, char* puerto);
void cargar_configuracion(char* config_path);

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Uso: %s <config_path> <id_cpu>\n", argv[0]);
        return EXIT_FAILURE;
    }

    cargar_configuracion(argv[1]);
    cpu_id = strdup(argv[2]);

    logger = log_create("cpu.log", "CPU", true, LOG_LEVEL_INFO);
    log_info(logger, "Iniciando CPU %s", cpu_id);

    int dispatch_fd = conectar_a_kernel(ip_kernel, puerto_dispatch);
    int interrupt_fd = conectar_a_kernel(ip_kernel, puerto_interrupt);

    if (dispatch_fd == -1 || interrupt_fd == -1) {
        log_error(logger, "No se pudo conectar a Kernel.");
        return EXIT_FAILURE;
    }

    // Handshake inicial (podrías mandar ID)
    send(dispatch_fd, cpu_id, strlen(cpu_id) + 1, 0);
    send(interrupt_fd, cpu_id, strlen(cpu_id) + 1, 0);

    while (1) {
        // TODO: implementar recibir instrucciones
        sleep(1);
    }

    close(dispatch_fd);
    close(interrupt_fd);

    log_destroy(logger);
    config_destroy(config);
    free(cpu_id);

    return EXIT_SUCCESS;
}

void cargar_configuracion(char* config_path) {
    config = config_create(config_path);
    ip_kernel = strdup(config_get_string_value(config, "IP_KERNEL"));
    puerto_dispatch = strdup(config_get_string_value(config, "PUERTO_KERNEL_DISPATCH"));
    puerto_interrupt = strdup(config_get_string_value(config, "PUERTO_KERNEL_INTERRUPT"));
}

int conectar_a_kernel(char* ip, char* puerto) {
    struct addrinfo hints, *server_info;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    getaddrinfo(ip, puerto, &hints, &server_info);
    int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

    if (connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) != 0) {
        freeaddrinfo(server_info);
        return -1;
    }

    freeaddrinfo(server_info);
    return socket_cliente;
}
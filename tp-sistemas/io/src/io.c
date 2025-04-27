#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <commons/config.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

t_log* logger;
t_config* config;
char* ip_kernel;
char* puerto_kernel;
char* nombre_dispositivo;

void cargar_configuracion(char* config_path);

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Uso: %s <config_path> <nombre_dispositivo>\n", argv[0]);
        return EXIT_FAILURE;
    }

    cargar_configuracion(argv[1]);
    nombre_dispositivo = argv[2];

    logger = log_create("io.log", "IO", true, LOG_LEVEL_INFO);
    log_info(logger, "Iniciando IO %s", nombre_dispositivo);

    struct addrinfo hints, *server_info;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    getaddrinfo(ip_kernel, puerto_kernel, &hints, &server_info);
    int socket_kernel = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

    if (connect(socket_kernel, server_info->ai_addr, server_info->ai_addrlen) != 0) {
        log_error(logger, "No se pudo conectar al Kernel");
        return EXIT_FAILURE;
    }
    freeaddrinfo(server_info);

    send(socket_kernel, nombre_dispositivo, strlen(nombre_dispositivo) + 1, 0);

    while (1) {
        // TODO: recibir peticiones de IO
        sleep(1);
    }

    close(socket_kernel);
    log_destroy(logger);
    config_destroy(config);

    return EXIT_SUCCESS;
}

void cargar_configuracion(char* config_path) {
    config = config_create(config_path);
    ip_kernel = strdup(config_get_string_value(config, "IP_KERNEL"));
    puerto_kernel = strdup(config_get_string_value(config, "PUERTO_KERNEL"));
}
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
char* puerto_escucha;

void cargar_configuracion(char* config_path);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Uso: %s <config_path>\n", argv[0]);
        return EXIT_FAILURE;
    }

    cargar_configuracion(argv[1]);
    logger = log_create("memoria.log", "MEMORIA", true, LOG_LEVEL_INFO);

    log_info(logger, "Iniciando Memoria en puerto %s", puerto_escucha);

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in direccion;
    direccion.sin_family = AF_INET;
    direccion.sin_addr.s_addr = INADDR_ANY;
    direccion.sin_port = htons(atoi(puerto_escucha));

    bind(server_fd, (struct sockaddr*)&direccion, sizeof(direccion));
    listen(server_fd, SOMAXCONN);

    while (1) {
        int cliente_fd = accept(server_fd, NULL, NULL);
        log_info(logger, "Se conectó un módulo a Memoria");
        // TODO: implementar recepción de operaciones
    }

    close(server_fd);
    log_destroy(logger);
    config_destroy(config);

    return EXIT_SUCCESS;
}

void cargar_configuracion(char* config_path) {
    config = config_create(config_path);
    puerto_escucha = strdup(config_get_string_value(config, "PUERTO_ESCUCHA"));
}
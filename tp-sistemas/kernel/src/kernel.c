// kernel.c - Módulo Kernel 1/5/25 (agrego comunicacion.h)

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <commons/collections/dictionary.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <comunicacion.h>

// Definiciones de estructuras

typedef struct {
    int pid;
    int program_counter;
    t_list* metricas_estado;
    t_list* tiempos_estado;
} pcb_t;

// Definiciones globales
t_list* lista_new;
t_list* lista_ready;
t_list* lista_exec;
t_list* lista_blocked;
t_list* lista_susp_blocked;
t_list* lista_susp_ready;
t_list* lista_exit;
t_dictionary* dispositivos_io;

pthread_mutex_t mutex_new;
pthread_mutex_t mutex_ready;
pthread_mutex_t mutex_exec;
pthread_mutex_t mutex_blocked;
pthread_mutex_t mutex_susp_blocked;
pthread_mutex_t mutex_susp_ready;
pthread_mutex_t mutex_exit;

pthread_t hilo_servidor_dispatch;
pthread_t hilo_servidor_interrupt;

int socket_escucha_dispatch;
int socket_escucha_interrupt;

char* ip_memoria;
char* puerto_memoria;
char* puerto_dispatch;
char* puerto_interrupt;

t_log* logger;
t_config* config;

// Funciones auxiliares
int iniciar_servidor(char* puerto);
int esperar_cliente(int socket_servidor);
void* atender_dispatch(void* args);
void* atender_interrupt(void* args);
void cargar_configuracion();
void inicializar_kernel();
void destruir_kernel();
int crear_proceso_en_memoria(int pid, int tamanio);

// Implementaciones

void cargar_configuracion() {
    config = config_create("kernel.config");

    ip_memoria = strdup(config_get_string_value(config, "IP_MEMORIA"));
    puerto_memoria = strdup(config_get_string_value(config, "PUERTO_MEMORIA"));
    puerto_dispatch = strdup(config_get_string_value(config, "PUERTO_ESCUCHA_DISPATCH"));
    puerto_interrupt = strdup(config_get_string_value(config, "PUERTO_ESCUCHA_INTERRUPT"));
}

void inicializar_kernel() {
    logger = log_create("kernel.log", "KERNEL", true, LOG_LEVEL_INFO);
    cargar_configuracion();

    lista_new = list_create();
    lista_ready = list_create();
    lista_exec = list_create();
    lista_blocked = list_create();
    lista_susp_blocked = list_create();
    lista_susp_ready = list_create();
    lista_exit = list_create();
    dispositivos_io = dictionary_create();

    pthread_mutex_init(&mutex_new, NULL);
    pthread_mutex_init(&mutex_ready, NULL);
    pthread_mutex_init(&mutex_exec, NULL);
    pthread_mutex_init(&mutex_blocked, NULL);
    pthread_mutex_init(&mutex_susp_blocked, NULL);
    pthread_mutex_init(&mutex_susp_ready, NULL);
    pthread_mutex_init(&mutex_exit, NULL);

    log_info(logger, "Kernel inicializado.");
}

void destruir_kernel() {
    list_destroy_and_destroy_elements(lista_new, free);
    list_destroy_and_destroy_elements(lista_ready, free);
    list_destroy_and_destroy_elements(lista_exec, free);
    list_destroy_and_destroy_elements(lista_blocked, free);
    list_destroy_and_destroy_elements(lista_susp_blocked, free);
    list_destroy_and_destroy_elements(lista_susp_ready, free);
    list_destroy_and_destroy_elements(lista_exit, free);
    dictionary_destroy_and_destroy_elements(dispositivos_io, free);

    pthread_mutex_destroy(&mutex_new);
    pthread_mutex_destroy(&mutex_ready);
    pthread_mutex_destroy(&mutex_exec);
    pthread_mutex_destroy(&mutex_blocked);
    pthread_mutex_destroy(&mutex_susp_blocked);
    pthread_mutex_destroy(&mutex_susp_ready);
    pthread_mutex_destroy(&mutex_exit);

    free(ip_memoria);
    free(puerto_memoria);
    free(puerto_dispatch);
    free(puerto_interrupt);

    config_destroy(config);
    log_destroy(logger);
}

int iniciar_servidor(char* puerto) {
    int socket_servidor;
    struct sockaddr_in direccion_servidor;

    socket_servidor = socket(AF_INET, SOCK_STREAM, 0);
    direccion_servidor.sin_family = AF_INET;
    direccion_servidor.sin_addr.s_addr = INADDR_ANY;
    direccion_servidor.sin_port = htons(atoi(puerto));

    bind(socket_servidor, (struct sockaddr*) &direccion_servidor, sizeof(direccion_servidor));
    listen(socket_servidor, SOMAXCONN);

    return socket_servidor;
}

int esperar_cliente(int socket_servidor) {
    struct sockaddr_in direccion_cliente;
    socklen_t tam_direccion = sizeof(struct sockaddr_in);
    return accept(socket_servidor, (struct sockaddr*)&direccion_cliente, &tam_direccion);
}

void* atender_dispatch(void* args) {
    int socket_cliente;
    socket_escucha_dispatch = iniciar_servidor(puerto_dispatch);
    log_info(logger, "Servidor Dispatch iniciado en puerto %s", puerto_dispatch);

    while (1) {
        socket_cliente = esperar_cliente(socket_escucha_dispatch);
        log_info(logger, "Se conectó una CPU al puerto Dispatch");
        // TODO: Manejar recepción y envío de procesos
    }
}

void* atender_interrupt(void* args) {
    int socket_cliente;
    socket_escucha_interrupt = iniciar_servidor(puerto_interrupt);
    log_info(logger, "Servidor Interrupt iniciado en puerto %s", puerto_interrupt);

    while (1) {
        socket_cliente = esperar_cliente(socket_escucha_interrupt);
        log_info(logger, "Se conectó una CPU al puerto Interrupt");
        // TODO: Manejar interrupciones
    }
}

// Conectar con memoria y pedir creación de proceso
int crear_proceso_en_memoria(int pid, int tamanio) {
    int socket_memoria;
    struct addrinfo hints, *server_info;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    getaddrinfo(ip_memoria, puerto_memoria, &hints, &server_info);
    socket_memoria = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

    if (connect(socket_memoria, server_info->ai_addr, server_info->ai_addrlen) != 0) {
        log_error(logger, "No se pudo conectar a Memoria.");
        freeaddrinfo(server_info);
        return -1;
    }

    freeaddrinfo(server_info);

    int codigo_operacion = 1; // 1 = INIT_PROCESS (a definir mejor en protocolos después)
    send(socket_memoria, &codigo_operacion, sizeof(int), 0);
    send(socket_memoria, &pid, sizeof(int), 0);
    send(socket_memoria, &tamanio, sizeof(int), 0);

    int respuesta;
    recv(socket_memoria, &respuesta, sizeof(int), MSG_WAITALL);

    close(socket_memoria);

    return respuesta; // 0 = OK, -1 = Error
}

// Función principal
int main(int argc, char* argv[]) {
    inicializar_kernel();

    pthread_create(&hilo_servidor_dispatch, NULL, atender_dispatch, NULL);
    pthread_create(&hilo_servidor_interrupt, NULL, atender_interrupt, NULL);

    pthread_join(hilo_servidor_dispatch, NULL);
    pthread_join(hilo_servidor_interrupt, NULL);

    destruir_kernel();
    return 0;
}

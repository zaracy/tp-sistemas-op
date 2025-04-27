# tp-sistemas-op
TP SSOO 1c 2025

1er checkpoint:

## Compilación
- Ingresar a cada carpeta (kernel/, cpu/, memoria/, io/)
- Ejecutar make all

## Ejecución
1. Primero levantar Memoria:
    ./memoria
2. Luego levantar Kernel:
    ./kernel
3. Después levantar IO:
    ./io
4. Finalmente levantar CPU:
    ./cpu

## Descripción
- Todos los módulos levantan sockets.
- Se establece conexión inicial.
- Kernel puede crear procesos y pedir a Memoria.
- CPU se conecta a Kernel para recibir futuros procesos.
- IO se conecta a Kernel para gestionar dispositivos.

## Requisitos
- Tener so-commons-library clonada dentro de /commons/.

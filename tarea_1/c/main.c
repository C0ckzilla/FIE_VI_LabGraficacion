#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define NUM_HILOS 4
#define TAM_ARR 6

typedef struct {
    int id_hilo;
    int cantidad_generada;
    char nombre_archivo[50];
} datos_hilo;

// Prototipos de funciones
void *funcion_hilo(void *arg);
int verificar(char *arreglo);
void escribir_archivo(char *nombre_archivo, char *arreglo);

void *funcion_hilo(void *arg) {
    datos_hilo *datos = (datos_hilo *)arg;
    int generados = 0;
    char arreglo[TAM_ARR + 1];

    while (1) { // Bucle para generar continuamente hasta que se cumpla una condición de parada
        // Generar arreglo aquí, asegurarse de que no haya caracteres contiguos iguales
        // ...

        if (verificar(arreglo)) {
            escribir_archivo(datos->nombre_archivo, arreglo);
            generados++;
        }

        // Condición de parada (podría ser una cantidad de arreglos o un tiempo determinado)
        // ...
    }

    datos->cantidad_generada = generados;
    pthread_exit(NULL);
}

int verificar(char *arreglo) {
    // Comprobar que no haya caracteres iguales contiguos
    // ...
    return 1; // Retornar 1 si es válido, 0 de lo contrario
}

void escribir_archivo(char *nombre_archivo, char *arreglo) {
    FILE *archivo = fopen(nombre_archivo, "a");
    if (archivo != NULL) {
        fputs(arreglo, archivo);
        fputs("\n", archivo);
        fclose(archivo);
    } else {
        fprintf(stderr, "Error al abrir el archivo %s\n", nombre_archivo);
    }
}

int main() {
    pthread_t hilos[NUM_HILOS];
    datos_hilo datos[NUM_HILOS];
    int i;

    // Iniciar contador de tiempo
    clock_t inicio = clock();

    // Crear y ejecutar hilos
    for (i = 0; i < NUM_HILOS; i++) {
        datos[i].id_hilo = i;
        sprintf(datos[i].nombre_archivo, "hilo%d.txt", i);
        if (pthread_create(&hilos[i], NULL, funcion_hilo, &datos[i]) != 0) {
            perror("Failed to create thread");
        }
    }

    // Esperar a que los hilos terminen
    for (i = 0; i < NUM_HILOS; i++) {
        pthread_join(hilos[i], NULL);
    }

    // Finalizar contador de tiempo y calcular duración
    clock_t fin = clock();
    double tiempo_transcurrido = (double)(fin - inicio) / CLOCKS_PER_SEC;

    // Imprimir resultados
    printf("Tiempo de ejecución: %.2f segundos\n", tiempo_transcurrido);
    for (i = 0; i < NUM_HILOS; i++) {
        printf("Hilo %d generó %d arreglos válidos.\n", i, datos[i].cantidad_generada);
    }

    return 0;
}

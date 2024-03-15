// Facultad de Ingeniería Eléctrica - Ingeniería en Computación UMSNH
// Materia: Laboratorio de Graficación 2024-2024
// Profesor: Ing. César Dionicio Arreola Rodríguez
// Alumno: Octavio Antonio Juárez Romero
// Matrícula: 1577250a

// Tarea #1: Generador de arreglos hexadecimales.
// Este programa genera arreglos de caracteres aleatorios y los escribe en archivos de texto.
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <direct.h> // Para _getcwd en entornos específicos de Windows
#define NUM_THREADS 4
#define ARRAY_SIZE 7 // 6 posiciones + 1 para el caracter nulo
#define MAX_ARRAY_SIZE 1000
#define MAX_CONTIGUOS 2


// Prototipos de funciones
void *threadFunction(void *threadid);
int verificar(char *arreglo);
void generarHexadecimal(char *arreglo);
void escribirArchivo(char *arreglo, int threadID);

void generarHexadecimal(char *arreglo) {
    const char hexChars[] = "0123456789abcdef";
    for (int i = 0; i < ARRAY_SIZE - 1; i++) { // Deja espacio para el caracter nulo
        int randIndex = rand() % 16; // Genera un índice aleatorio para hexChars
        arreglo[i] = hexChars[randIndex];
    }
    arreglo[ARRAY_SIZE - 1] = '\0'; // Asegura que el arreglo sea una cadena válida
}

int verificar(char *arreglo) {
    int contador = 1; // Inicia el contador para el primer carácter.

    for (int i = 0; i < ARRAY_SIZE - 2; i++) { // Ajuste en el límite para comparación segura.
        if (arreglo[i] == arreglo[i + 1]) {
            // Incrementa el contador si el carácter actual es igual al siguiente.
            contador++;
            // Verifica si el contador excede el máximo de caracteres contiguos permitidos.
            if (contador > MAX_CONTIGUOS) {
                return 0; // El arreglo no es válido si se excede el límite.
            }
        } else {
            // Reinicia el contador si el carácter actual es diferente al siguiente.
            contador = 1;
        }
    }

    return 1; // Retorna 1 si el arreglo cumple con la restricción de contigüidad.
}

void escribirArchivo(char *arreglo, int threadID) {
    char filename[20];
    sprintf(filename, "hilo%d.txt", threadID); // Crea un nombre de archivo único para cada hilo
    FILE *file = fopen(filename, "a"); // Abre el archivo en modo de añadir
    if (file != NULL) {
        fprintf(file, "%s\n", arreglo); // Escribe el arreglo en el archivo
        fclose(file); // Cierra el archivo
    } else {
        fprintf(stderr, "Error abriendo el archivo %s\n", filename);
    }
}

void *threadFunction(void *threadid) {
    long threadID = (long)(intptr_t)threadid;
    char arreglo[ARRAY_SIZE];

    for (int i = 0; i < MAX_ARRAY_SIZE; i++) { // Ejemplo: cada hilo intenta generar 1000 arreglos
        generarHexadecimal(arreglo);
        if (verificar(arreglo)) {
            escribirArchivo(arreglo, threadID);
        }
    }
    pthread_exit(NULL);
}

int main() {
    char cwd[FILENAME_MAX];
    if (_getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Directorio de trabajo actual: %s\n", cwd);
    } else {
        perror("Error al obtener el directorio de trabajo actual");
        return EXIT_FAILURE;
    }
    srand(time(NULL)); // Inicializa el generador de números aleatorios

    pthread_t threads[NUM_THREADS];
    int rc;
    long t;

    clock_t start = clock();

    for(t = 0; t < NUM_THREADS; t++) {
        rc = pthread_create(&threads[t], NULL, threadFunction, (void *)(intptr_t)t);
        if (rc) {
            printf("ERROR; el código de retorno de pthread_create() es %d\n", rc);
            exit(-1);
        }
    }

    for(t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Tiempo de ejecución: %f segundos\n", time_spent);

    pthread_exit(NULL);
}

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_THREADS 4
#define ARRAY_SIZE 7  // 6 caracteres hexadecimales + '\0'
const char HEX_DIGITS[] = "0123456789abcdef";

void *generateAndVerify(void *threadid);
void inicializarArregloSegunHilo(char *arreglo, long tid);
void incrementarArreglo(char *arreglo);
int verificar(const char *arreglo);
int finalDelEspacioDeBusqueda(const char *arreglo, long tid);

void *generateAndVerify(void *threadid) {
    long tid = (long)threadid;
    char arreglo[ARRAY_SIZE];
    
    inicializarArregloSegunHilo(arreglo, tid);

    char filename[20];
    snprintf(filename, sizeof(filename), "hilo_%ld.txt", tid);
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error al abrir el archivo");
        pthread_exit(NULL);
    }

    do {
        if (verificar(arreglo)) {
            fprintf(file, "%s\n", arreglo);
        }
        incrementarArreglo(arreglo);
    } while (!finalDelEspacioDeBusqueda(arreglo, tid));

    fclose(file);
    pthread_exit(NULL);
}

void inicializarArregloSegunHilo(char *arreglo, long tid) {
    memset(arreglo, '0', ARRAY_SIZE - 1);
    arreglo[ARRAY_SIZE - 1] = '\0';
    int segmento = 16 / NUM_THREADS;
    arreglo[0] = HEX_DIGITS[tid * segmento];
}

void incrementarArreglo(char *arreglo) {
    for (int i = ARRAY_SIZE - 2; i > 0; --i) {
        if (arreglo[i] < 'f') {
            arreglo[i]++;
            if (i < ARRAY_SIZE - 2 && arreglo[i] == arreglo[i + 1]) {
                arreglo[i] = '0';
            } else {
                break;
            }
        } else {
            arreglo[i] = '0';
        }
    }
}

int verificar(const char *arreglo) {
    for (int i = 0; i < ARRAY_SIZE - 2; i++) {
        if (arreglo[i] == arreglo[i + 1]) {
            return 0;
        }
    }
    return 1;
}

int finalDelEspacioDeBusqueda(const char *arreglo, long tid) {
    int segmento = 16 / NUM_THREADS;
    char maxValor = HEX_DIGITS[((tid + 1) * segmento) - 1];
    return arreglo[0] > maxValor;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int rc;

    for (long t = 0; t < NUM_THREADS; t++) {
        rc = pthread_create(&threads[t], NULL, generateAndVerify, (void *)t);
        if (rc) {
            printf("ERROR; el código de retorno de pthread_create() es %d\n", rc);
            exit(-1);
        }
    }

    for (int t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    printf("Todos los arreglos han sido generados y verificados.\n");

    return 0;
}

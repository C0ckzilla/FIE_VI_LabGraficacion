#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

int main ()
{
    srand(time(0)); // Mueve la inicialización del generador de números aleatorios fuera del bucle
    float target;
    int found = -1;

    printf("Ingrese el elemento a buscar: ");
    scanf("%d", &target);
    
    #pragma omp parallel
    {
        int hilos = omp_get_num_threads();
        int actual = omp_get_thread_num();

        float arreglo[6];

        #pragma omp for
        for (int i = 0;  i <6; i++) {
            arreglo[i] = (float)rand() / (float)RAND_MAX * 9;
            //printf("arreglo[%d] = %f desde hilo %d de %d\n", i, arreglo[i], actual, hilos);
        

            if (arreglo[i] == target) 
            {
                #pragma omp critical
                {
                found = i;
                printf("Elemento %d encontrado en la posición %d por el hilo %d\n", target, found, actual);
                }

            } 
        }

         // Imprime el arreglo completo generado por cada hilo
        printf("Arreglo generado por el hilo %d: \n", actual);
        for (int i = 0; i < 6; i++) {
            printf(" %.2f ", arreglo[i]);
        }
        printf("\n");

    }
    
  

    
    return 0;
}
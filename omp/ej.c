#include <stdio.h>
#include <omp.h>
int main ()
{
    /**/
    #pragma omp parallel
    {
        int hilos = omp_get_num_threads();
        int actual = omp_get_thread_num();
        printf("Hola mundo desde el hilo %d de %d\n", actual, hilos);
        
        
            #pragma omp for
            for (int i = 0; i < 16; i++) {
                printf("Valor de i = %d, calculado por el hilo %d\n", i, actual);
            }
        
    }
}
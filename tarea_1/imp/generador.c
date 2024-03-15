#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 7 // 6 posiciones + 1 para el caracter nulo

void generarHexadecimal(char *arreglo) {
    const char hexChars[] = "0123456789abcdef";
    for (int i = 0; i < ARRAY_SIZE - 1; i++) { // Deja espacio para el caracter nulo
        int randIndex = rand() % (sizeof(hexChars) - 1); // Excluye el carácter nulo del final
        arreglo[i] = hexChars[randIndex];
    }
    arreglo[ARRAY_SIZE - 1] = '\0'; // Asegura que el arreglo sea una cadena válida
}

#define MAX_CONTIGUOS 1 // Máximo número de caracteres contiguos iguales permitidos

int verificar(const char *arreglo) {
    for (int i = 0; i < ARRAY_SIZE - 2; i++) { // Verifica hasta el penúltimo carácter
        int count = 1; // Inicia el contador para el primer carácter
        
        // Cuenta caracteres contiguos iguales
        while (i < ARRAY_SIZE - 2 && arreglo[i] == arreglo[i + 1]) {
            count++;
            i++;
            
            if (count > MAX_CONTIGUOS) {
                return 0; // Falso si se excede el máximo de caracteres contiguos iguales
            }
        }
    }
    return 1; // Verdadero si el arreglo es válido
}


int main() {
    char arreglo[ARRAY_SIZE];
    int esValido = 0;
    
    // Inicializa el generador de números aleatorios
    srand(time(NULL));
    
    // Genera y muestra arreglos hasta que uno sea válido
    do {
        generarHexadecimal(arreglo);
        esValido = verificar(arreglo);
    } while (!esValido);
    
    printf("Arreglo Hexadecimal Válido: %s\n", arreglo);
    
    return 0;
}


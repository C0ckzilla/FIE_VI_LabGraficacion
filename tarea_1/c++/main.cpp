#include <iostream>
#include <string>
#include <thread>
#include <fstream>
#include <vector>
#include <random>
#include <chrono>

#define MAX_CONTIGUOS 1
#define NUM_THREADS 4
#define ARRAY_SIZE 7 // Incluyendo el carácter nulo

// Generador de números aleatorios
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> distrib(0, 15);

void generarHexadecimal(std::string& arreglo) {
    const char hexChars[] = "0123456789ABCDEF";
    arreglo.clear();
    for (int i = 0; i < ARRAY_SIZE - 1; ++i) { // Dejamos espacio para el carácter nulo automáticamente manejado por std::string
        arreglo += hexChars[distrib(gen)];
    }
}

bool verificar(const std::string& arreglo) {
    int contador = 1;
    for (size_t i = 0; i < arreglo.length() - 1; ++i) {
        if (arreglo[i] == arreglo[i + 1]) {
            contador++;
            if (contador > MAX_CONTIGUOS) return false;
        } else {
            contador = 1;
        }
    }
    return true;
}

void escribirArchivo(const std::string& arreglo, int threadID) {
    std::string filename = "hilo" + std::to_string(threadID) + ".txt";
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        file << arreglo << std::endl;
        file.close();
    } else {
        std::cerr << "Error abriendo el archivo " << filename << std::endl;
    }
}

void threadFunction(int threadID) {
    std::string arreglo;
    for (int i = 0; i < 1000; ++i) {
        generarHexadecimal(arreglo);
        if (verificar(arreglo)) {
            escribirArchivo(arreglo, threadID);
        }
    }
}

int main() {
    std::vector<std::thread> threads;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back(threadFunction, i);
    }

    for (auto& th : threads) {
        th.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Tiempo de ejecución: " << elapsed.count() << " segundos" << std::endl;

    return 0;
}

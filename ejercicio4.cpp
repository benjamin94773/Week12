#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <mutex>

std::mutex m;

void buscarValor(const std::vector<int>& v, int valor, int id) {
    bool encontrado = std::find(v.begin(), v.end(), valor) != v.end();
    std::lock_guard<std::mutex> lock(m);
    std::cout << "Vector " << id << ": ";
    if (encontrado)
        std::cout << "El valor " << valor << " fue encontrado.\n";
    else
        std::cout << "El valor " << valor << " no está presente.\n";
}

int main() {
    std::vector<std::vector<int>> vectores = {
        {1, 3, 5, 7},
        {2, 4, 6, 8},
        {10, 20, 30}
    };

    int valor = 5;
    std::vector<std::thread> hilos;

    for (size_t i = 0; i < vectores.size(); ++i) {
        hilos.emplace_back(buscarValor, vectores[i], valor, i + 1);
    }

    for (auto& t : hilos) {
        t.join();
    }

    return 0;
}

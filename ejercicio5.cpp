#include <iostream>
#include <thread>
#include <vector>
#include <cmath>
#include <mutex>

const double g = 9.81;
std::mutex mutex_cout;

void simularCaida(double altura, int id) {
    double tiempo = std::sqrt((2 * altura) / g);
    std::lock_guard<std::mutex> lock(mutex_cout);
    std::cout << "Objeto " << id << " (altura = " << altura << " m) -> tiempo de caida = "
              << tiempo << " s\n";
}

int main() {
    std::vector<double> alturas = {10, 50, 100, 250};
    std::vector<std::thread> hilos;

    for (size_t i = 0; i < alturas.size(); ++i) {
        hilos.emplace_back(simularCaida, alturas[i], i + 1);
    }

    for (auto& t : hilos) {
        t.join();
    }

    return 0;
}

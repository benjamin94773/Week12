#include <iostream>
#include <thread>
#include <vector>
#include <numeric>
#include <mutex>

std::mutex mtx;

void sumarVector(const std::vector<int>& v, int id) {
    int suma = std::accumulate(v.begin(), v.end(), 0);
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "Vector " << id << " -> suma = " << suma << "\n";
}

int main() {
    std::vector<std::vector<int>> vectores = {
        {1, 2, 3},
        {10, 20, 30},
        {5, 5, 5, 5}
    };

    std::vector<std::thread> hilos;

    for (size_t i = 0; i < vectores.size(); ++i) {
        hilos.emplace_back(sumarVector, vectores[i], i + 1);
    }

    for (auto& t : hilos) {
        t.join();
    }

    return 0;
}

#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <string>
#include <fstream>

std::mutex mtx;

// Estructura para representar una imagen PPM
struct ImagenPPM {
    int ancho, alto;
    std::vector<unsigned char> pixeles; // RGB
    
    bool cargar(const std::string& archivo) {
        std::ifstream file(archivo, std::ios::binary);
        if(!file) return false;
        
        std::string formato;
        file >> formato;
        if(formato != "P6" && formato != "P3") return false;
        
        file >> ancho >> alto;
        int max_val;
        file >> max_val;
        file.ignore(256, '\n');
        
        pixeles.resize(ancho * alto * 3);
        
        if(formato == "P6") {
            file.read(reinterpret_cast<char*>(pixeles.data()), pixeles.size());
        } else {
            for(size_t i = 0; i < pixeles.size(); i++) {
                int val;
                file >> val;
                pixeles[i] = val;
            }
        }
        
        return true;
    }
    
    bool guardar(const std::string& archivo) {
        std::ofstream file(archivo, std::ios::binary);
        if(!file) return false;
        
        file << "P6\n" << ancho << " " << alto << "\n255\n";
        file.write(reinterpret_cast<char*>(pixeles.data()), pixeles.size());
        
        return true;
    }
    
    void aplicarFiltroBlancoNegro() {
        for(size_t i = 0; i < pixeles.size(); i += 3) {
            // Calcular escala de grises
            unsigned char r = pixeles[i];
            unsigned char g = pixeles[i + 1];
            unsigned char b = pixeles[i + 2];
            unsigned char gris = (r + g + b) / 3;
            
            // Aplicar umbral: > 128 -> blanco, sino -> negro
            unsigned char valor = (gris > 128) ? 255 : 0;
            
            pixeles[i] = valor;
            pixeles[i + 1] = valor;
            pixeles[i + 2] = valor;
        }
    }
};

// Función que procesa una imagen completa: Leer -> Aplicar filtro -> Guardar
void procesarImagen(const std::string& archivo_entrada, const std::string& archivo_salida) {
    ImagenPPM imagen;
    
    // PASO 1: LEER IMAGEN
    if(!imagen.cargar(archivo_entrada)) {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "[ERROR] No se pudo cargar: " << archivo_entrada << std::endl;
        std::cout << "        Verifique que el archivo sea formato PPM valido\n\n";
        return;
    }
    
    {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "[OK] Imagen leida: " << archivo_entrada 
                  << " (" << imagen.ancho << "x" << imagen.alto << " pixeles)\n";
    }
    
    // PASO 2: APLICAR FILTRO BLANCO Y NEGRO
    imagen.aplicarFiltroBlancoNegro();
    
    {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "[OK] Filtro aplicado: " << archivo_entrada 
                  << " (" << (imagen.ancho * imagen.alto) << " pixeles procesados)\n";
    }
    
    // PASO 3: GUARDAR IMAGEN
    if(imagen.guardar(archivo_salida)) {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "[OK] Imagen guardada: " << archivo_salida << "\n\n";
    } else {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "[ERROR] No se pudo guardar: " << archivo_salida << "\n\n";
    }
}

int main() {
    std::cout << "===============================================================================\n";
    std::cout << "  PROCESAMIENTO PARALELO DE IMAGENES - FILTRO BLANCO Y NEGRO\n";
    std::cout << "===============================================================================\n\n";
    
    // Definir imágenes a procesar (entrada -> salida)
    std::vector<std::pair<std::string, std::string>> imagenes = {
        {"sulfur.ppm", "sulfur_filtrada.ppm"},
        {"damma.ppm", "damma_filtrada.ppm"}
    };
    
    auto inicio = std::chrono::high_resolution_clock::now();
    
    std::vector<std::thread> hilos;
    
    // Crear un hilo para cada imagen (procesamiento paralelo)
    for (const auto& img : imagenes) {
        hilos.emplace_back(procesarImagen, img.first, img.second);
    }
    
    // Esperar a que todos los hilos terminen
    for (auto& t : hilos) {
        t.join();
    }
    
    auto fin = std::chrono::high_resolution_clock::now();
    auto duracion = std::chrono::duration_cast<std::chrono::milliseconds>(fin - inicio);
    
    std::cout << "===============================================================================\n";
    std::cout << "  RESULTADOS DEL PROCESAMIENTO\n";
    std::cout << "===============================================================================\n";
    std::cout << "Tiempo total de procesamiento: " << duracion.count() << " ms\n";
    std::cout << "Imagenes procesadas: " << imagenes.size() << "\n";
    std::cout << "Archivos generados:\n";
    for (const auto& img : imagenes) {
        std::cout << "  - " << img.second << "\n";
    }
    std::cout << "===============================================================================\n";
    
    return 0;
}

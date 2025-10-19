# Algoritmos Embarazosamente Paralelos

Este trabajo implementa cuatro algoritmos embarazosamente paralelos en C++ utilizando programación paralela con hilos (std::thread).

## Requisitos

- Docker con la imagen: `japeto/parallel-tools:v64`

## Compilación y Ejecución

### 1. Acceder al contenedor Docker

```bash
docker run -it -v "${PWD}:/home/japeto/app:rw" -w /home/japeto/app japeto/parallel-tools:v64 bash
```

### 2. Compilar y ejecutar cada ejercicio

Una vez dentro del contenedor Docker, ejecutar los siguientes comandos:

#### Ejercicio 1: Filtro de Imágenes
```bash
g++ ejercicio1.cpp -o ejercicio1 -pthread
./ejercicio1
```

Procesa las imágenes `sulfur.ppm` y `damma.ppm`, aplicando un filtro blanco y negro y generando las versiones filtradas.

#### Ejercicio 3: Suma de Vectores
```bash
g++ ejercicio3.cpp -o ejercicio3 -pthread
./ejercicio3
```

Calcula la suma de elementos en múltiples vectores de forma paralela.

#### Ejercicio 4: Búsqueda en Vectores
```bash
g++ ejercicio4.cpp -o ejercicio4 -pthread
./ejercicio4
```

Busca un valor específico en múltiples vectores de manera paralela.

#### Ejercicio 5: Simulación de Caída Libre
```bash
g++ ejercicio5.cpp -o ejercicio5 -pthread
./ejercicio5
```

Calcula el tiempo de caída libre para múltiples objetos desde diferentes alturas usando la fórmula física t = √(2h/g).

## Descripción de los Ejercicios

### Ejercicio 1: Procesamiento de Imágenes
- **Entrada**: Archivos PPM (sulfur.ppm, damma.ppm)
- **Proceso**: Aplicación de filtro blanco y negro mediante umbralización (threshold = 128)
- **Salida**: Archivos PPM filtrados (sulfur_filtrada.ppm, damma_filtrada.ppm)
- **Paralelización**: Cada imagen se procesa en un hilo independiente

### Ejercicio 3: Suma de Vectores
- **Entrada**: Múltiples vectores de enteros
- **Proceso**: Suma de elementos usando std::accumulate
- **Salida**: Suma total de cada vector
- **Paralelización**: Cada vector se suma en un hilo independiente

### Ejercicio 4: Búsqueda en Vectores
- **Entrada**: Múltiples vectores y un valor a buscar
- **Proceso**: Búsqueda lineal usando std::find
- **Salida**: Indicación de presencia/ausencia del valor en cada vector
- **Paralelización**: Cada búsqueda se realiza en un hilo independiente

### Ejercicio 5: Caída Libre
- **Entrada**: Alturas de múltiples objetos
- **Proceso**: Cálculo del tiempo de caída usando física clásica
- **Salida**: Tiempo de caída para cada objeto
- **Paralelización**: Cada cálculo se realiza en un hilo independiente

## Características Técnicas

- **Lenguaje**: C++11
- **Paralelización**: std::thread
- **Sincronización**: std::mutex para proteger salida estándar
- **Patrón**: Algoritmos embarazosamente paralelos (sin dependencias entre tareas)

## Archivos del Proyecto

```
.
├── ejercicio1.cpp          # Filtro de imágenes
├── ejercicio3.cpp          # Suma de vectores
├── ejercicio4.cpp          # Búsqueda en vectores
├── ejercicio5.cpp          # Simulación de caída libre
├── sulfur.ppm             # Imagen de entrada 1
├── damma.ppm              # Imagen de entrada 2
└── README.md              # Este archivo
```

## Notas

- Los programas utilizan mutex para evitar condiciones de carrera al escribir en stdout
- La paralelización es "embarazosamente paralela" porque cada tarea es completamente independiente

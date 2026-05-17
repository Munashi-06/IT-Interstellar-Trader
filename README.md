# IT-Interstellar-Trader

Proyecto de videojuego para **Programación 3** desarrollado en C++ utilizando la librería **SFML**.

## Desarrollo (Cómo clonar y compilar el código)
Hemos estandarizado la compilación utilizando Makefiles y MSYS2/Linux para que todos los desarrolladores tengan el mismo entorno. Sigue estas instrucciones:

### Requisitos Previos e Instalación

**Windows (Usando MSYS2 - RECOMENDADO)**
1. Descarga e instala [MSYS2](https://www.msys2.org/).
2. Abre la terminal **MSYS2 UCRT64** (es importante que sea la de color amarillo que dice UCRT64).
3. Ejecuta el siguiente comando para instalar el control de versiones (Git), el compilador, CMake y SFML precompilado:
   - pacman -S git mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-make mingw-w64-ucrt-x86_64-sfml

**macOS**

Asegúrate de tener Homebrew instalado y ejecuta en la terminal:
- xcode-select --install
- brew install cmake sfml

**Linux**

Dependiendo de la distribución de Linux que utilices, el gestor de paquetes y los nombres de las dependencias cambiarán. Abre tu terminal y ejecuta el comando correspondiente a tu sistema:

* **Para Ubuntu / Debian / Pop!_OS:**
  - `sudo apt update && sudo apt install -y build-essential cmake libx11-dev libxrandr-dev libudev-dev libopengl-dev libflac-dev libvorbis-dev libopenal-dev libfreetype-dev libxcursor-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev git`

* **Para Arch Linux / Manjaro / EndeavourOS:**
  - `sudo pacman -Syu base-devel cmake libx11 libxrandr systemd flac libvorbis openal freetype2 libxcursor libxi mesa glu git`

* **Para Fedora:**
  - `sudo dnf install gcc-c++ make cmake libX11-devel libXrandr-devel systemd-devel flac-devel libvorbis-devel openal-soft-devel freetype-devel libXcursor-devel libXi-devel mesa-libGL-devel mesa-libGLU-devel git`

### Pasos para Clonar y Compilar
1. Clonar el repositorio
   - git clone [https://github.com/Munashi-06/IT-Interstellar-Trader.git](https://github.com/Munashi-06/IT-Interstellar-Trader.git)
   - cd IT-Interstellar-Trader
2. Crear el directorio de construcción y entrar en él:
   - `mkdir build`
   - `cd build`
3. Configurar el proyecto con CMake:
   - En Windows (MSYS2 UCRT64):
      - `cmake -G "MinGW Makefiles" ..`
   - En Linux / MacOS:
      - `cmake ..`
4. Compilar el ejecutable:
   - `cmake --build .`
5. Ejecutar el Juego
   - **En Windows**:
      - `./IT-Interstellar-Trader.exe`
   - **En Linux / MacOS**:
      - `./IT-Interstellar-Trader`

   **NOTA CRÍTICA: Asegúrate de que la carpeta assets/ se encuentre en el mismo directorio de trabajo que el ejecutable generado (/build/assets/), de lo contrario, el motor fallará al cargar las texturas y fuentes.**
---
## Arquitectura y Estructuras de Datos (Core Data Structures)
La arquitectura de este proyecto se basa en estructuras de datos personalizadas para optimizar el rendimiento y el uso de memoria:

1. **Radar de Prioridad (Max-Heap)**

Los eventos planetarios (Guerras, Hambrunas, Plagas) se gestionan mediante un Max-Heap personalizado almacenado secuencialmente en un espacio de memoria contiguo (std::vector<Planet>).

- **Mapeo del Arreglo**: Para cualquier nodo en el índice i, su padre se ubica en (i - 1) / 2. Sus hijos en 2i + 1 y 2i + 2.

- **Criterio de Ordenamiento**: La función cmp prioriza eventos galácticos críticos activos sobre mercados planetarios estables para mantener un flujo de radar enfocado en emergencias.

2. **Inventario de la Nave (Flyweight Pattern & Optional Slots)**

Para evitar la sobrecarga de memoria, la asignación del almacenamiento separa los datos estáticos pesados de la mercancía dinámica:

- `std::vector<std::optional<ItemStack>>`: Una colección de Slots fijos que representa la capacidad máxima de la bodega. El uso de std::nullopt define de manera segura un espacio vacío.

- **Catálogo Flyweight**: Las propiedades de los objetos (categorías, valores base, texturas) viven globalmente dentro de una tabla Hash (std::unordered_map). El ItemStack solo transporta el string del ID y métricas financieras localizadas.

3. **Árboles de Mejoras Tecnológicas (Binary Search Trees)**

Las mejoras de la nave están organizadas en 3 Árboles Binarios independientes: Propulsión, Logística y Comercio.

- Al seleccionar una mejora con ramas mutuamente exclusivas, el algoritmo recorre el árbol y marca automáticamente el nodo hermano directo como `BLOCKED_BY_CHOICE`, sellando esa rama tecnológica por el resto de la partida e impidiendo fugas de memoria por mejoras incompatibles.
## Estructura del Proyecto
El código está separado en archivos de cabecera (`.hpp`) y de implementación (`.cpp`), organizados internamente por módulos para mantener una arquitectura limpia a medida que el juego escala:

- `include/`: Contiene todos los archivos de cabecera (`.hpp`).
- `src/`: Contiene todos los archivos de código fuente (`.cpp`).
  - **Core/**: El núcleo del motor (Game Loop, inicialización, administrador de estados globales).
  - **Entities/**: Los actores concretos del universo (Player, Planet, Item).
  - **Systems/**: La lógica del juego y el procesamiento de entidades (Economy, TradeManager, Inventory).
  - **Interface/**: La capa visual y los menús con los que interactúa el jugador.
  - **DataStructures/**: Estructuras de datos puras y algoritmos de soporte (como los montículos, árboles binarios o futuras tablas hash).
- `assets/`: Imágenes, sonidos y fuentes del juego.
- `CMakeLists.txt`: Configuración de construcción y dependencias.

---

## Equipo de Desarrollo (Escuadrón Alpha Buena Maravilla Buena Onda Lobo Salvaje)
- Angel Garcia
- Reinier Manrique
- Abraham Vergara
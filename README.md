# IT-Interstellar-Trader

Proyecto de videojuego para **Programación 3** desarrollado en C++ utilizando la librería **SFML**.

## Requisitos Previos
Antes de clonar y compilar, asegúrate de tener instalado:

### Windows
1. **Visual Studio Build Tools 2022/2026**: Instalar el paquete "Desarrollo para el escritorio con C++" (incluye MSVC y CMake).
2. **CMake**: Asegúrate de que esté en el PATH del sistema.

### Linux (Ubuntu/Debian)
Ejecuta el siguiente comando para instalar las dependencias necesarias de SFML y herramientas de compilación:
- sudo apt update && sudo apt install -y build-essential cmake libx11-dev libxrandr-dev libudev-dev libopengl-dev libflac-dev libvorbis-dev libopenal-dev

## Cómo Compilar y Ejecutar
Sigue estos pasos en tu terminal (PowerShell en Windows o Bash en Linux) dentro de la carpeta del proyecto:
1. **Crear carpeta de compilación:**
   - mkdir build
   - cd build
2. **Configurar el proyecto (Descarga SFML automáticamente):**
   - cmake ..
3. **Compilar el codigo:**
   - cmake --build . --config Debug
4. **Ejecutar el Juego:**
   - Windows:
      - cd Debug
      - .\IT-Interstellar-Trader.exe
   - Linux:
      - ./IT-Interstellar-Trader

## Estructura del Proyecto
El código está separado en archivos de cabecera (`.hpp`) y de implementación (`.cpp`), organizados internamente por módulos para mantener una arquitectura limpia a medida que el juego escala:

- `include/`: Contiene todos los archivos de cabecera (`.hpp`).
- `src/`: Contiene todos los archivos de código fuente (`.cpp`).
  - **Core/**: El núcleo del motor (Game Loop, inicialización, administrador de estados globales).
  - **Entities/**: Los actores concretos del universo (Player, Planet, Item).
  - **Systems/**: La lógica del juego y el procesamiento de entidades (Economy, TradeManager, Inventory).
  - **Interface/**: La capa visual y los menús con los que interactúa el jugador.
  - **Utils/**: Estructuras de datos puras y algoritmos de soporte (como los montículos, árboles binarios o futuras tablas hash).
- `assets/`: Imágenes, sonidos y fuentes del juego.
- `CMakeLists.txt`: Configuración de construcción y dependencias.

## Equipo (Grupo de 3)
- Angel Garcia
- Reinier Manrique
- Abraham Vergara
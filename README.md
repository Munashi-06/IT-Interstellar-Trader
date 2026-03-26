# IT-Interstellar-Trader

Proyecto de videojuego para **Programación 3** desarrollado en C++ utilizando la librería **SFML**.

## Requisitos Previos
Antes de clonar y compilar, asegúrate de tener instalado:

### Windows
1. **Visual Studio Build Tools 2022/2026**: Instalar el paquete "Desarrollo para el escritorio con C++" (incluye MSVC y CMake).
2. **CMake**: Asegúrate de que esté en el PATH del sistema.

### Linux (Ubuntu/Debian)
Ejecuta el siguiente comando para instalar las dependencias necesarias de SFML y herramientas de compilación:
```bash
sudo apt update && sudo apt install -y build-essential cmake libx11-dev libxrandr-dev libudev-dev libopengl-dev libflac-dev libvorbis-dev libopenal-dev

#### Cómo Compilar y Ejecutar
Sigue estos pasos en tu terminal (PowerShell en Windows o Bash en Linux) dentro de la carpeta del proyecto:
1. **Crear carpeta de compilación:**
   mkdir build
   cd build
2. **Configurar el proyecto (Descarga SFML automáticamente):**
   cmake ..
3. **Compilar el codigo:**
   cmake --build . --config Debug
4. **Ejecutar el Juego:**
   - Windows:
      .\Debug\IT-Interstellar-Trader.exe
   - Linux:
      ./IT-Interstellar-Trader


##### Estructura del Proyecto
- src/: Archivos fuente (.cpp, .hpp).
- assets/: Imágenes, sonidos y fuentes del juego.
- CMakeLists.txt: Configuración de construcción y dependencias.

###### Equipo (Grupo de 3)
- Angel Garcia
- Reinier Manrique
- Abraham Vergara
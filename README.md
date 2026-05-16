# IT-Interstellar-Trader

Proyecto de videojuego para **Programación 3** desarrollado en C++ utilizando la librería **SFML**.

---

## Cómo Jugar (Solo Ejecutar)
Si solo quieres probar el juego sin instalar herramientas de programación, busca la última versión en la pestaña **Releases** de este repositorio y descarga el archivo comprimido.

### Solución de problemas comunes (Windows):
Si al hacer doble clic en `IT-Interstellar-Trader.exe` el juego no abre o lanza un error indicando que faltan archivos `.dll` (como `VCRUNTIME140.dll` o similares), significa que tu PC necesita las librerías base de C++. 
* **Solución:** Descarga e instala el [Microsoft Visual C++ Redistributable](https://aka.ms/vs/17/release/vc_redist.x64.exe) (oficial de Microsoft) y vuelve a abrir el juego.

---

## Desarrollo (Cómo clonar y compilar el código)
Hemos estandarizado la compilación utilizando Makefiles y MSYS2/Linux para que todos los desarrolladores tengan el mismo entorno. Sigue estas instrucciones:

### Requisitos Previos e Instalación

**Windows (Usando MSYS2 - RECOMENDADO)**
1. Descarga e instala [MSYS2](https://www.msys2.org/).
2. Abre la terminal **MSYS2 UCRT64** (es importante que sea la de color amarillo que dice UCRT64).
3. Ejecuta el siguiente comando para instalar el control de versiones (Git), el compilador, CMake y SFML precompilado:
   - pacman -S git mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-make mingw-w64-ucrt-x86_64-sfml

**Linux (Ubuntu/Debian)**

Abre tu terminal y ejecuta el siguiente comando para instalar Git, el compilador, CMake y las dependencias de SFML:

- sudo apt update && sudo apt install -y build-essential cmake libx11-dev libxrandr-dev libudev-dev libopengl-dev libflac-dev libvorbis-dev libopenal-dev libfreetype-dev libxcursor-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev git

### Pasos para Clonar el Repositorio

Ve a la raiz de la carpeta donde quieras guardar el proyecto y ejecuta:
   - git clone https://github.com/Munashi-06/IT-Interstellar-Trader.git
   
### Pasos para Compilar
Una vez tengas las herramientas instaladas, navega con la terminal hasta la carpeta raíz del repositorio clonado y ejecuta los siguientes comandos:
1. Crear el directorio de construcción y entrar en él:
   - mkdir build
   - cd build
2. Configurar el proyecto con CMake:
   - En Windows (MSYS2 UCRT64):
      - cmake -G "MinGW Makefiles" ..
   - En Linux:
      - cmake ..
3. Compilar el código:
   - cmake --build .
4. Ejecutar el Juego
   - **En Windows**:
      - ./IT-Interstellar-Trader.exe
   - **En Linux**:
      - ./IT-Interstellar-Trader

---

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

---

## Equipo
- Angel Garcia
- Reinier Manrique
- Abraham Vergara
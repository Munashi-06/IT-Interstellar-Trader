# IT-Interstellar-Trader

Proyecto de videojuego para **Programación 3** desarrollado en C++ utilizando la librería **SFML**.

---

## Cómo Jugar (Solo Ejecutar)
Si solo quieres probar el juego sin instalar herramientas de programación, busca la última versión en la pestaña **Releases** de este repositorio y descarga el archivo comprimido.

### Solución de problemas comunes (Windows):
Si al hacer doble clic en `IT-Interstellar-Trader.exe` el juego no abre o lanza un error indicando que faltan archivos `.dll` (como `VCRUNTIME140.dll` o similares), significa que tu PC necesita las librerías base de C++. 
* **Solución:** Descarga e instala el [Microsoft Visual C++ Redistributable](https://aka.ms/vs/17/release/vc_redist.x64.exe) (oficial de Microsoft) y vuelve a abrir el juego.

---

## Desarrollo (Cómo compilar el código)
Si deseas clonar el repositorio y compilar el código fuente por tu cuenta, sigue estas instrucciones:

### Requisitos Previos

**Windows**
1. Tener instalado **Visual Studio Build Tools 2022** con el paquete "Desarrollo para el escritorio con C++" (incluye MSVC y CMake).
2. **CMake**: Asegúrate de que esté agregado al PATH del sistema.

**Linux (Ubuntu/Debian)**
Ejecuta el siguiente comando para instalar el compilador, CMake y todas las dependencias multimedia que requiere SFML:

- sudo apt update && sudo apt install -y build-essential cmake libx11-dev libxrandr-dev libudev-dev libopengl-dev libflac-dev libvorbis-dev libopenal-dev libfreetype-dev libxcursor-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev git

### Pasos para Compilar
#### En Linux:
1. **Crear carpeta de compilación:**
   - mkdir build
   - cd build
2. **Configurar el proyecto (Descarga SFML automáticamente):**
   - cmake ..
3. **Compilar el codigo:**
   - cmake --build . --config Release
4. **Ejecutar el Juego:**
   - ./IT-Interstellar-Trader

#### En Windows:
1. Abre el menú inicio de Windows, busca y abre el **Developer PowerShell for VS 2022**.
2. Navega con el comando `cd` hasta la carpeta raíz del proyecto.
3. Ejecuta los siguientes comandos uno por uno:
   - mkdir build
   - cd build
   - cmake -G "Visual Studio 17 2022" ..
   - cmake --build . --config Release
4. Ejecuta el programa
   - cd Release
   - .\IT-Interstellar-Trader.exe

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
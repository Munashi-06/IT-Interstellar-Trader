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
1. **Visual Studio Build Tools**: Instalar el paquete "Desarrollo para el escritorio con C++" (incluye el compilador MSVC y CMake).
2. **CMake**: Asegúrate de que esté agregado al PATH del sistema.

**Linux (Ubuntu/Debian)**
Ejecuta el siguiente comando para instalar el compilador, CMake y todas las dependencias multimedia que requiere SFML:

- sudo apt update && sudo apt install -y build-essential cmake libx11-dev libxrandr-dev libudev-dev libopengl-dev libflac-dev libvorbis-dev libopenal-dev libfreetype6-dev

### Pasos para Compilar
Ejecuta estos comandos en tu terminal (PowerShell en Windows o Bash en Linux) dentro de la carpeta raíz del proyecto:
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
BUILD_DIR = build
EXE_NAME = IT-Interstellar-Trader.exe
# En Windows usamos la ruta con Debug, en Linux sería solo $(BUILD_DIR)/$(EXE_NAME)
EXE_PATH = $(BUILD_DIR)/Debug/$(EXE_NAME)

.PHONY: all setup build run clean

# Comando por defecto: Configura, compila y corre
all: setup build run

# 1. Crear carpeta y configurar CMake
setup:
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake ..

# 2. Compilar
build:
	cmake --build $(BUILD_DIR) --config Debug

# 3. Ejecutar
run:
	cd $(BUILD_DIR)/Debug && .\$(EXE_NAME)

# 4. Limpiar la compilación
clean:
	@if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
	@echo Proyecto limpio.
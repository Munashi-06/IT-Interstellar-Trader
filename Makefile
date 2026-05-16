BUILD_DIR = build
EXE_NAME = IT-Interstellar-Trader

ifeq ($(OS),Windows_NT)
    EXE_FILE = $(EXE_NAME).exe
    CMAKE_GEN = -G "MinGW Makefiles"
else
    EXE_FILE = $(EXE_NAME)
    CMAKE_GEN =
endif

.PHONY: all setup build run clean test

all: setup build run

setup:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake $(CMAKE_GEN) ..

build:
	cmake --build $(BUILD_DIR)

run:
	cd $(BUILD_DIR) && ./$(EXE_FILE)

test: build run

clean:
	rm -rf $(BUILD_DIR)
	@echo "Proyecto limpio."
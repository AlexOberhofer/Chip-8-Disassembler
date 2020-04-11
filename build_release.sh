#!/bin/bash

BIN1=./bin/chip8
BIN2=./bin/chip8.exe
BIN_DIR=./bin/
WINDOWS_SDL_LIB_PATH=./res/SDL2.dll
CLEANUP=1 #TODO add cleanup flag logic
RELEASE_DIR=./release/

#Attempt clean
echo "Attempting to clean project..."

if [ -d "$BIN_DIR" ]; then
    echo "Cleaning binary directory..."
    rm -rf "$BIN_DIR"
fi

if [ -d "$RELEASE_DIR" ]; then  
    echo "Cleaning release directory..."
    rm -rf "$RELEASE_DIR"
fi

#Build Linux
echo "Compiling linux version from source..."
make

#Build windows
echo "Compiling windows version from source..."
make -f makefile.windows


#Package Linux
echo "Packaging Linux..."
mkdir -p bin/linux/sys
mkdir -p bin/linux/rom
mkdir -p bin/linux/config

echo "Zipping Linux Release..."
zip -rq chip8_linux.zip ./bin/linux

#Package windows
echo "Packaging Windows..."

if [ ! -f "$WINDOWS_SDL_LIB_PATH" ]; then
    echo "WARNING: NO SDL2 DDL FOUND IN CONFIGURED LIB PATH: "$WINDOWS_SDL_LIB_PATH"!!!"
    echo "SDL2 library will not be included in the release for windows."
else 
    cp "$WINDOWS_SDL_LIB_PATH" ./bin/win/SDL2.dll
fi

mkdir -p bin/win/sys
mkdir -p bin/win/rom
mkdir -p bin/win/config

echo "Zipping Windows Release..."
zip -rq chip8_windows.zip ./bin/win

#Move to release directory
mkdir -p release
mkdir -p release/unix/
mkdir -p release/windows/

mv chip8_windows.zip ./release/windows/chip8_windows.zip
mv chip8_linux.zip ./release/unix/linux.zip

echo "Script completed successfully!"
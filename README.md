# A Chip 8 Implementation in C and SDL2

A Chip 8 Emulator/Interpreter written in C with an SDL2 Front end.

<p align="center">
  <img src="https://github.com/AlexOberhofer/Chip-8-Emulator/raw/master/doc/screenshot.png" alt="Chip 8 Image"/>
</p>

## Prerequisites 

Building this emulator requires [GNU Make](https://www.gnu.org/software/make/ "Make"),
 [GCC](https://gcc.gnu.org/install/ "GCC"), and [SDL2](https://wiki.libsdl.org/Installation "SDL2"). A
 build script is provided for GNU/Linux Systems, but this program can be compiled for Windows/MacOS/ARM Systems, etc.

## Building

Clone this repository

```
$ git clone https://github.com/AlexOberhofer/Chip-8-Emulator.git
```
Switch to working directory

```
$ cd Chip-8-Emulator
```

Build

```
$ make
```


## Usage

Normal Usage
 
```
$ <path/to/chip8> <path/to/rom>
```

Debug Usage

This emulator offers simple terminal debugging by use of a debug flag. When the
debug flag is set via command line, a simple cycle by cycle dump is printed to
the console. By default this displays all registers and special registers, PC, 
and opcode for each CPU cycle. A memory dump can be added to this by recompiling the
binary with memory dumping enabled.

```
$ <path/to/chip8> --debug <path/to/rom>
```


## Controls

The hexidecimal keypad of the Chip 8 is mapped to the leftmost 4x4 keys on the keyboard.
For example, the top most row of the Chip 8 hexdecimal keyboard maps to the keys 1, 2, 3, 4. The second
row of the hexidecial keyboard maps to Q, W, E, R, etc. 

The emulator also offers a small number of additional controls.

```
P - PAUSE GAME
```

```
R - RESUME GAME
```

```
ESC - QUIT 
```

## Authors

 **Alex Oberhofer**
 
## Comments and Ackowledgements

This project has been a side project of mine that I developed over the Summer of 2018
in an attempt to learn more about hardware emulation. The emulator works with most roms, however
there still may be bugs and the code was not optomized after I wrote the initial logic. If anyone 
can improve upon this emulator please submit a pull request and it will be considered for
addition to the emulator.

I would also like to acklowedge some resources I used during this process.

The BC_Chip8Test ROM was used to fix bugs in the CPU logic and is listed in this repository
along with the associated text file documentation in the docs directory. The license information
for this ROM is also documented in this file.

[Emulator 101](http://www.emulator101.com/ "Emulator 101") - A great resource for those interested in writing their own emulator.

[Cowgod Chip 8 Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM "Cowgod Reference") - An informative documentation of the Chip 8 virtual machine.

[Mastering Chip 8](http://mattmik.com/files/chip8/mastering/chip8.html "Mastering Chip 8") - More Chip 8 Documenation.


## License

This project is licensed under the GNU GPL 3.0 License - see the [LICENSE.md](LICENSE.md) file for details

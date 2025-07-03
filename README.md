# Chip-8-Emulator

I built a CHIP-8 "emulator" that replicates the behavior of a simple 8-bit virtual machine. It emulates the following components:
- Monochrome pixel display (64×32 resolution)
- 16 8-bit registers + an index register
- Call stack
- All standard CHIP-8 opcodes/instructions
- 16-key keypad input

## See it in action
<table> <tr> <td align="center"> <strong>ROM Menu</strong><br> <img src="https://github.com/TheRealGecko/Chip-8-Emulator/blob/61cb57a4f9ed982664bf5c1fada0a8e32d16884c/demo_gifs/chip8_rom_menu.gif" width="400"/> </td> <td align="center" style="padding-bottom: 10px;"> <strong>Pong</strong><br> <img src="https://github.com/TheRealGecko/Chip-8-Emulator/blob/61cb57a4f9ed982664bf5c1fada0a8e32d16884c/demo_gifs/chip8_pong.gif" width="400"/> </td> </tr> <tr> <td align="center"> <strong>Tetris</strong><br> <img src="https://github.com/TheRealGecko/Chip-8-Emulator/blob/61cb57a4f9ed982664bf5c1fada0a8e32d16884c/demo_gifs/chip8_tetris.gif" width="400"/> </td> <td align="center"> <strong>Lunar Lander</strong><br> <img src="https://github.com/TheRealGecko/Chip-8-Emulator/blob/61cb57a4f9ed982664bf5c1fada0a8e32d16884c/demo_gifs/chip8_lunar_lander.gif" width="400"/> </td> </tr> </table>

*Note: I wrote the emulator, not the ROM/game files. The ROM files can be found here ([source](https://github.com/kripod/chip8-roms/tree/master/games))!*

## What is Chip-8?
Chip-8 is an interpreted language designed for 8-bit systems in the mid-1970s. Since writing an interpreter for it closely resembles the process of writing emulators in general, I figured it would be a great way to try out emu-dev!

## Key Inputs

### Global Controls
- `ESC` - Quit the program at any time  
- `V` - Return to the ROM menu when in a game  

### Menu Navigation
- `Q`/`E` - Switch between ROMs  
- `W` - Select ROM  

### Pong Controls
- `1`/`2` - Move up/down  

### Tetris Controls
- `W`/`E` - Move left/right  
- `Q` - Rotate  

### Lunar Lander Controls
- `Q`/`E` - Move left/right  
- `2` - Thrust up  

## To-Do
- Create a physical/hand-held version of CHIP-8
- Add sound emulation

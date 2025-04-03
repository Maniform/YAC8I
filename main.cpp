#define OLC_PGE_APPLICATION
#include <C8EnginePE.h>

#include <print>

//#define TEST_ROM "C:/Users/nvill/OneDrive/Documents/Projects/Cpp/C8Engine/roms/chip8-roms/demos/Maze (alt) [David Winter, 199x].ch8"
//#define TEST_ROM "C:/Users/nvill/OneDrive/Documents/Projects/Cpp/C8Engine/roms/chip8-roms/demos/Maze [David Winter, 199x].ch8"
//#define TEST_ROM "C:/Users/nvill/OneDrive/Documents/Projects/Cpp/C8Engine/roms/chip8-roms/demos/Particle Demo [zeroZshadow, 2008].ch8"
//#define TEST_ROM "C:/Users/nvill/OneDrive/Documents/Projects/Cpp/C8Engine/roms/chip8-roms/demos/Trip8 Demo (2008) [Revival Studios].ch8"
//#define TEST_ROM "C:/Users/nvill/OneDrive/Documents/Projects/Cpp/C8Engine/roms/chip8-roms/demos/Zero Demo [zeroZshadow, 2007].ch8"

//#define TEST_ROM "C:/Users/nvill/OneDrive/Documents/Projects/Cpp/C8Engine/roms/chip8-roms/games/15 Puzzle [Roger Ivie] (alt).ch8"
//#define TEST_ROM "C:/Users/nvill/OneDrive/Documents/Projects/Cpp/C8Engine/roms/chip8-roms/games/15 Puzzle [Roger Ivie].ch8"
//#define TEST_ROM "C:/Users/nvill/OneDrive/Documents/Projects/Cpp/C8Engine/roms/chip8-roms/games/Addition Problems [Paul C. Moews].ch8"
//#define TEST_ROM "C:/Users/nvill/OneDrive/Documents/Projects/Cpp/C8Engine/roms/chip8-roms/games/Tic-Tac-Toe [David Winter].ch8"

//#define TEST_ROM "/Users/noel/Documents/Projets/YAC8I/3rdParty/chip8-roms/games/Addition Problems [Paul C. Moews].ch8"
//#define TEST_ROM "/Users/noel/Documents/Projets/YAC8I/3rdParty/chip8-roms/demos/Maze (alt) [David Winter, 199x].ch8"
//#define TEST_ROM "/Users/noel/Documents/Projets/YAC8I/3rdParty/chip8-roms/demos/Trip8 Demo (2008) [Revival Studios].ch8"
#define TEST_ROM "/Users/noel/Documents/Projets/YAC8I/3rdParty/chip8-roms/demos/Particle Demo [zeroZshadow, 2008].ch8"

int main(int argc, char** argv)
{
    try
    {
		C8EnginePE demo;
		
		const string rom = argc > 1 ? argv[1] : TEST_ROM;
		demo.loadRomFromFile(rom.c_str());
		
        while(demo.update())
        {
        }
    }
    catch(const std::exception& e)
    {
        printf("Error : %s\n", e.what());
    }

	return 0;
}

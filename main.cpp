#define OLC_PGE_APPLICATION
#include <C8Engine.h>

#define PIXEL_WIDTH 10

//#define TEST_ROM "C:/Users/nvill/OneDrive/Documents/Projects/Cpp/C8Engine/roms/chip8-roms/demos/Maze (alt) [David Winter, 199x].ch8"
//#define TEST_ROM "C:/Users/nvill/OneDrive/Documents/Projects/Cpp/C8Engine/roms/chip8-roms/demos/Maze [David Winter, 199x].ch8"
//#define TEST_ROM "C:/Users/nvill/OneDrive/Documents/Projects/Cpp/C8Engine/roms/chip8-roms/demos/Particle Demo [zeroZshadow, 2008].ch8"
//#define TEST_ROM "C:/Users/nvill/OneDrive/Documents/Projects/Cpp/C8Engine/roms/chip8-roms/demos/Trip8 Demo (2008) [Revival Studios].ch8"
//#define TEST_ROM "C:/Users/nvill/OneDrive/Documents/Projects/Cpp/C8Engine/roms/chip8-roms/demos/Zero Demo [zeroZshadow, 2007].ch8"

//#define TEST_ROM "C:/Users/nvill/OneDrive/Documents/Projects/Cpp/C8Engine/roms/chip8-roms/games/15 Puzzle [Roger Ivie] (alt).ch8"
//#define TEST_ROM "C:/Users/nvill/OneDrive/Documents/Projects/Cpp/C8Engine/roms/chip8-roms/games/15 Puzzle [Roger Ivie].ch8"
#define TEST_ROM "C:/Users/nvill/OneDrive/Documents/Projects/Cpp/C8Engine/roms/chip8-roms/games/Addition Problems [Paul C. Moews].ch8"
//#define TEST_ROM "C:/Users/nvill/OneDrive/Documents/Projects/Cpp/C8Engine/roms/chip8-roms/games/Tic-Tac-Toe [David Winter].ch8"

int main(int argc, char** argv)
{
	C8Engine demo;
//	if (demo.Construct(SCREEN_WIDTH, SCREEN_HEIGHT, PIXEL_WIDTH, PIXEL_WIDTH * 2))
//	{
		const string rom = argc > 1 ? argv[1] : TEST_ROM;
//		demo.loadRomFromFile(rom.c_str());

//	}

	return 0;
}

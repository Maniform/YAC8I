#define OLC_PGE_APPLICATION
#if defined(_WIN32) && !defined(_DEBUG)
#include <Windows.h>
#endif
#include <C8EnginePE.h>

#include <iostream>

#define TEST_ROM "demos/Maze (alt) [David Winter, 199x].ch8"
//#define TEST_ROM "demos/Maze [David Winter, 199x].ch8"
//#define TEST_ROM "demos/Particle Demo [zeroZshadow, 2008].ch8"
//#define TEST_ROM "demos/Trip8 Demo (2008) [Revival Studios].ch8"
//#define TEST_ROM "demos/Zero Demo [zeroZshadow, 2007].ch8"

//#define TEST_ROM "games/15 Puzzle [Roger Ivie] (alt).ch8"
//#define TEST_ROM "games/15 Puzzle [Roger Ivie].ch8"
//#define TEST_ROM "games/Addition Problems [Paul C. Moews].ch8"
//#define TEST_ROM "games/Tic-Tac-Toe [David Winter].ch8"

int main(int argc, char** argv)
{
	try
	{
#if defined(_WIN32) && !defined(_DEBUG)
		FreeConsole();
#endif

		C8EnginePE demo;
		
		const string rom = argc > 1 ? argv[1] : string(ROMS_FOLDER) + TEST_ROM;
		demo.loadRomFromFile(rom.c_str());
		
		while(demo.update())
		{
		}
	}
	catch(const std::exception& e)
	{
		cerr << "Error : " << e.what() << endl;
	}

	return 0;
}

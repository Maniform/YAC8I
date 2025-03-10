#pragma once

#include <olcPixelGameEngine.h>
#include <vector>
#include <span>
#include <random>
#include <stack>

using namespace std;

#define SCREEN_WIDTH	64
#define SCREEN_HEIGHT	32
#define FREQUENCY		4,194304 //MHz
#define RAM_SIZE		4096 //bytes

class C8Engine : public olc::PixelGameEngine
{
public:
	C8Engine();

public:
	bool OnUserCreate() override;

	bool OnUserUpdate(float fElapsedTime) override;

	void loadRomFromFile(const char* filename);

private:
	vector<uint8_t> ram;

	span<uint8_t> rom;

	//Program Counter
	uint16_t pc;

	//Registers
	uint16_t I;
	stack<uint16_t> stack;
	uint8_t DT; // Delay timer
	uint8_t ST; // Sound timer
	uint8_t V[16]; // Variables
	span<uint8_t> F; // Font
	const vector<olc::Key> keys;
	const vector<uint8_t> keyMap;
	uint16_t pkb; //previous keyboard state
	uint16_t kb;

	bool running;

	mt19937 gen;
	uniform_int_distribution<> dis;

	bool framelimit;

	void execute(const uint16_t opcode);

	uint8_t getValue(const uint16_t value, const uint8_t position) const;
	uint8_t* getVx(const uint16_t opcode);
	uint8_t* getVy(const uint16_t opcode);
	uint8_t getKey(const uint16_t kbValue) const;

	uint8_t readPixel(int x, int y) const;
	void updateKeyboard();
	void updateTimers();
};

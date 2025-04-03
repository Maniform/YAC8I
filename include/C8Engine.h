#pragma once

#include <vector>
#include <span>
#include <random>
#include <stack>

using namespace std;

#define SCREEN_WIDTH	64
#define SCREEN_HEIGHT	32
#define RAM_SIZE		4096 //bytes
#define FRAMERATE		60

class C8Engine
{
public:
	C8Engine();

public:
    virtual bool update();
    
    void loadRomFromFile(const char* filename);
	
	void setFramelimitEnabled(bool enabled);
	bool isFramelimitEnabled() const;
    
protected:
    
    virtual void clearScreen() = 0;
    virtual void writePixel(uint8_t x, uint8_t y, bool state) = 0;
    virtual bool readPixel(uint8_t x, uint8_t y) const = 0;

private:
	vector<uint8_t> ram;

	span<uint8_t> rom;

	//Program Counter
	uint16_t pc;

	//Registers
	uint16_t I;
	stack<uint16_t> callstack;
	uint8_t DT; // Delay timer
	uint8_t ST; // Sound timer
	uint8_t V[16]; // Variables
	span<uint8_t> F; // Font
	const vector<unsigned int> keys;
	const vector<uint8_t> keyMap;
	uint16_t pkb; //previous keyboard state
	uint16_t kb;

	bool running;

	mt19937 gen;
	uniform_int_distribution<> dis;

	bool framelimit;
    
    virtual void updateKeyboard() = 0;
    void updateTimers();

    void execute(const uint16_t opcode);

	uint8_t getValue(const uint16_t value, const uint8_t position) const;
	uint8_t* getVx(const uint16_t opcode);
	uint8_t* getVy(const uint16_t opcode);
	uint8_t getKey(const uint16_t kbValue) const;
};

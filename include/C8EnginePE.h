#pragma once

#include <C8Engine.h>

#include <PixelEngine.h>

class C8EnginePE : public C8Engine
{
public:
	static C8EnginePE* engine;
	
	C8EnginePE();
	
	virtual bool update() override;
	
	virtual void clearScreen() override;
	virtual void writePixel(uint8_t x, uint8_t y, bool state) override;
	virtual bool readPixel(uint8_t x, uint8_t y) const override;
	
private:
	PixelEngine pe;
	double time;
	const vector<unsigned int> keys;
	
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	virtual void updateKeyboard() override;
	virtual void buzz() override;
};

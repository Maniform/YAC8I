#pragma once

#include <C8Engine.h>

#include <PixelEngine.h>

class C8EnginePE : public C8Engine
{
public:
	C8EnginePE();
	
	virtual bool update() override;
	
	virtual void clearScreen() override;
	virtual void writePixel(uint8_t x, uint8_t y, bool state) override;
	virtual bool readPixel(uint8_t x, uint8_t y) const override;
	
private:
	PixelEngine pe;
	double time;
	
	virtual void updateKeyboard() override;
};

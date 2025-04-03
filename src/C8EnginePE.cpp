#include <C8EnginePE.h>

#include <chrono>
#include <thread>

using namespace std;
using namespace glm;

C8EnginePE::C8EnginePE()
	: C8Engine()
	, pe(SCREEN_WIDTH, SCREEN_HEIGHT)
	, time(0.0)
{
	time = glfwGetTime();
}

bool C8EnginePE::update()
{
	bool running = true;
	
	running &= C8Engine::update();
	running &= pe.update();
	
	if(isFramelimitEnabled())
	{
		this_thread::sleep_for(chrono::microseconds(static_cast<long long>(1e6/FRAMERATE)));
		time = glfwGetTime();
	}
	
	return running;
}

void C8EnginePE::clearScreen()
{
	pe.clear();
}

void C8EnginePE::writePixel(uint8_t x, uint8_t y, bool state)
{
	pe.setPixelColor(x, y, state ? vec3(1.0f, 1.0f, 1.0f) : vec3(0.0f, 0.0f, 0.0f));
}

bool C8EnginePE::readPixel(uint8_t x, uint8_t y) const
{
	const vec3 pixel = pe.getPixelColor(x, y);
	return (pixel.x + pixel.y + pixel.z) > 0;
}

void C8EnginePE::updateKeyboard()
{
	
}

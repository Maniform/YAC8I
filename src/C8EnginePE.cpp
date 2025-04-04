#include <C8EnginePE.h>

#include <chrono>
#include <thread>
#include <GLFW/glfw3.h>

#ifdef __APPLE__
#include <AudioToolbox/AudioServices.h>
#else
#include <iostream>
#endif

using namespace glm;

C8EnginePE* C8EnginePE::engine(nullptr);

C8EnginePE::C8EnginePE()
	: C8Engine()
	, pe(SCREEN_WIDTH, SCREEN_HEIGHT)
	, time(0.0)
	, keys{
		GLFW_KEY_1, GLFW_KEY_2, GLFW_KEY_3, GLFW_KEY_4,
		GLFW_KEY_Q, GLFW_KEY_W, GLFW_KEY_E, GLFW_KEY_R,
		GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D, GLFW_KEY_F,
		GLFW_KEY_Z, GLFW_KEY_X, GLFW_KEY_C, GLFW_KEY_V
	}
	, previousOptionKeys{
		{ GLFW_KEY_L, GLFW_RELEASE }
	}
{
	engine = this;
	
	time = pe.getTime();
}

bool C8EnginePE::update()
{
	bool running = true;
	
	running &= C8Engine::update();
	running &= pe.update();
	
	if(isFramelimitEnabled())
	{
		this_thread::sleep_for(chrono::microseconds(static_cast<long long>(1e6/FRAMERATE)));
		time = pe.getTime();
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
	pkb = kb;

	for (uint8_t i = 0; i < keys.size(); i++)
	{
		switch (pe.getKey(keys[i]))
		{
			case GLFW_PRESS:
			case GLFW_REPEAT:
				kb |= 0b1 << keyMap[i];
				break;
				
			case GLFW_RELEASE:
				kb &= ~(0b1 << keyMap[i]);
				break;
		}
	}

	const GLuint key = pe.getKey(GLFW_KEY_L);
	if (key == GLFW_PRESS && key != previousOptionKeys[GLFW_KEY_L])
	{
		setFramelimitEnabled(!isFramelimitEnabled());
	}

	for (auto& previousOptionKey : previousOptionKeys)
	{
		previousOptionKey.second = pe.getKey(previousOptionKey.first);
	}
}

void C8EnginePE::buzz()
{
#ifdef __APPLE__
	AudioServicesPlayAlertSound(kSystemSoundID_UserPreferredAlert);
#else
	cout << "\007";
#endif
}

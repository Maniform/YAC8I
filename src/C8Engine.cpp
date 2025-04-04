#include <C8Engine.h>

#include <cstring>
#include <fstream>
#include <thread>

#define ROM_START 0x200
#define FONT_START 0x50

// Debug
//#define GB_DEBUG_PRINT

#ifdef GB_DEBUG_PRINT
#include <iostream>
#endif

C8Engine::C8Engine()
	: pkb(0)
	, kb(0)
	, keyMap{
		0x01, 0x02, 0x03, 0x0C,
		0x04, 0x05, 0x06, 0x0D,
		0x07, 0x08, 0x09, 0x0E,
		0x0A, 0x00, 0x0B, 0x0F
	}
	, pc(ROM_START)
	, I(0)
	, DT(0)
	, ST(0)
	, V{ 0 }
	, F()
	, running(true)
	, gen(random_device{}())
	, dis(0, 255)
	, framelimit(true)
{
	ram.resize(RAM_SIZE);
	rom = span<uint8_t>(ram.begin() + ROM_START, ram.size() - ROM_START);
	const vector<uint8_t> font = {
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};
	F = span<uint8_t>(ram.begin() + FONT_START, font.size());
	memcpy(F.data(), font.data(), font.size());
}

bool C8Engine::update()
{
	if ((pc + 1) < ram.size())
	{
		updateKeyboard();
		updateTimers();

		uint16_t opcode = (ram[pc] << 8) + ram[pc + 1];
		execute(opcode);
	}

	return running;
}

void C8Engine::loadRomFromFile(const char* filename)
{
	ifstream romFileStream;
	romFileStream.open(filename, ios::binary);
	romFileStream.seekg(ios::beg);
	romFileStream.unsetf(std::ios::skipws);
	if (romFileStream.is_open())
	{
		romFileStream.read(reinterpret_cast<char*>(rom.data()), rom.size());
		romFileStream.close();
	}
	else
	{
		throw runtime_error("can't open rom " + string(filename));
	}
}

void C8Engine::setFramelimitEnabled(bool enabled)
{
	framelimit = enabled;
}

bool C8Engine::isFramelimitEnabled() const
{
	return framelimit;
}

void C8Engine::execute(const uint16_t opcode)
{
#ifdef GB_DEBUG_PRINT
	printf("pc 0x%.4X op 0x%.2X ", pc, opcode);
#endif

	const uint8_t op1 = (opcode & 0xF000) >> 12;

	switch (op1)
	{
	case 0x0:
		switch (opcode)
		{
		case 0x00E0: // CLS
#ifdef GB_DEBUG_PRINT
			printf("CLS");
#endif
			clearScreen();
			pc += 2;
			break;

		case 0x00EE: // RET
#ifdef GB_DEBUG_PRINT
			printf("RET");
#endif
			pc = callstack.top();
			callstack.pop();
			break;

		default: // SYS addr
#ifdef GB_DEBUG_PRINT
			printf("SYS addr");
#endif
			pc = opcode & 0x0FFF;
			break;
		}
		break;

	case 0x1: // JP addr
#ifdef GB_DEBUG_PRINT
		printf("JP addr");
#endif
		pc = opcode & 0x0FFF;
		break;

	case 0x2: // CALL addr
#ifdef GB_DEBUG_PRINT
		printf("CALL addr");
#endif
		callstack.push(pc + 2);
		pc = opcode & 0x0FFF;
		break;

	case 0x3: // SE Vx, byte
	{
#ifdef GB_DEBUG_PRINT
		printf("SE Vx, byte");
#endif
		const uint8_t Vx = *getVx(opcode);
		const uint8_t kk = opcode & 0x00FF;
		if (Vx == kk)
		{
			pc += 2;
		}
		pc += 2;
		break;
	}

	case 0x4: // 4xkk - SNE Vx, byte
	{
#ifdef GB_DEBUG_PRINT
		printf("SNE Vx, byte");
#endif
		const uint8_t Vx = *getVx(opcode);
		const uint8_t kk = opcode & 0x00FF;
		if (Vx != kk)
		{
			pc += 2;
		}
		pc += 2;
		break;
	}

	case 0x5: // 5xy0 - SE Vx, Vy
	{
#ifdef GB_DEBUG_PRINT
		printf("SE Vx, Vy");
#endif
		const uint8_t Vx = *getVx(opcode);
		const uint8_t Vy = *getVy(opcode);
		if (Vx == Vy)
		{
			pc += 2;
		}
		pc += 2;
		break;
	}

	case 0x6: // 6xkk - LD Vx, byte
	{
#ifdef GB_DEBUG_PRINT
		printf("LD Vx, byte");
#endif
		uint8_t& Vx = *getVx(opcode);
		Vx = opcode & 0x00FF;
		pc += 2;
		break;
	}

	case 0x7: // 7xkk - ADD Vx, byte
	{
#ifdef GB_DEBUG_PRINT
		printf("ADD Vx, byte");
#endif
		uint8_t& Vx = *getVx(opcode);
		Vx += (opcode & 0x00FF);
		pc += 2;
		break;
	}

	case 0x8:
	{
		const uint8_t op000x = opcode & 0x000F;
		switch (op000x)
		{
		case 0x0: // 8xy0 - LD Vx, Vy
		{
#ifdef GB_DEBUG_PRINT
			printf("LD Vx, Vy");
#endif
			uint8_t& Vx = *getVx(opcode);
			const uint8_t Vy = *getVy(opcode);
			Vx = Vy;
			break;
		}

		case 0x1: // 8xy1 - OR Vx, Vy
		{
#ifdef GB_DEBUG_PRINT
			printf("OR Vx, Vy");
#endif
			uint8_t Vx = *getVx(opcode);
			const uint8_t Vy = *getVy(opcode);
			Vx = Vx | Vy;
			break;
		}

		case 0x2: // AND Vx, Vy
		{
#ifdef GB_DEBUG_PRINT
			printf("AND Vx, Vy");
#endif
			uint8_t& Vx = *getVx(opcode);
			const uint8_t Vy = *getVy(opcode);
			Vx = Vx & Vy;
			break;
		}

		case 0x3: // 8xy3 - XOR Vx, Vy
		{
#ifdef GB_DEBUG_PRINT
			printf("XOR Vx, Vy");
#endif
			uint8_t& Vx = *getVx(opcode);
			const uint8_t Vy = *getVy(opcode);
			Vx = Vx ^ Vy;
			break;
		}

		case 0x4: // ADD Vx, Vy
		{
#ifdef GB_DEBUG_PRINT
			printf("ADD Vx, Vy");
#endif
			uint8_t& Vx = *getVx(opcode);
			const uint8_t Vy = *getVy(opcode);
			uint16_t sum = Vx + Vy;
			V[0xF] = sum > 0xFF;
			Vx = sum & 0xFF;
			break;
		}

		case 0x5: // SUB Vx, Vy
		{
#ifdef GB_DEBUG_PRINT
			printf("SUB Vx, Vy");
#endif
			uint8_t& Vx = *getVx(opcode);
			const uint8_t Vy = *getVy(opcode);
			V[0xF] = Vx > Vy;
			Vx = Vx - Vy;
			break;
		}

		case 0x6: // 8xy6 - SHR Vx {, Vy}
		{
#ifdef GB_DEBUG_PRINT
			printf("SHR Vx {, Vy}");
#endif
			uint8_t& Vx = *getVx(opcode);
			V[0xF] = Vx & 0x1;
			Vx >>= 0x1;
			break;
		}

		case 0x7: // 8xy7 - SUBN Vx, Vy
		{
#ifdef GB_DEBUG_PRINT
			printf("SUBN Vx, Vy");
#endif
			uint8_t& Vx = *getVx(opcode);
			const uint8_t Vy = *getVy(opcode);
			V[0xF] = Vy > Vx;
			Vx = Vy - Vx;
			break;
		}

		case 0xE: // SHL Vx {, Vy}
		{
#ifdef GB_DEBUG_PRINT
			printf("SHL Vx {, Vy}");
#endif
			uint8_t& Vx = *getVx(opcode);
			V[0xF] = (Vx & 0b10000000) >> 7;
			Vx <<= 0x1;
			break;
		}

		default:
			printf("\nBad opcode : 0x%04X", opcode);
			break;
		}
		pc += 2;
		break;
	}

	case 0x9: // SNE Vx, Vy
	{
#ifdef GB_DEBUG_PRINT
		printf("SNE Vx, Vy");
#endif
		const uint8_t Vx = *getVx(opcode);
		const uint8_t Vy = *getVy(opcode);
		if (Vx != Vy)
		{
			pc += 2;
		}
		pc += 2;
		break;
	}

	case 0xA: // LD I, addr
#ifdef GB_DEBUG_PRINT
		printf("LD I, addr");
#endif
		I = opcode & 0x0FFF;
		pc += 2;
		break;

	case 0xB: // JP V0, addr
#ifdef GB_DEBUG_PRINT
		printf("JP V0, addr");
#endif
		pc = V[0] + (opcode & 0x0FFF);
		break;

	case 0xC: // RND Vx, byte
	{
#ifdef GB_DEBUG_PRINT
		printf("RND Vx, byte");
#endif
		uint8_t& Vx = *getVx(opcode);
		const uint8_t randomNumber = dis(gen);
		const uint8_t kk = opcode & 0x00FF;
		Vx = randomNumber & kk;
		pc += 2;
		break;
	}

	case 0xD: // DRW Vx, Vy, nibble
	{
#ifdef GB_DEBUG_PRINT
		printf("DRW Vx, Vy, nibble");
#endif
		const uint8_t& x = V[(opcode & 0x0F00) >> 8];
		const uint8_t& y = V[(opcode & 0x00F0) >> 4];
		uint8_t height = opcode & 0x000F;
		uint8_t pixel;

		V[0xF] = 0;
		for (int yline = 0; yline < height; yline++)
		{
			pixel = ram[I + yline];
			for (int xline = 0; xline < 8; xline++)
			{
				if ((pixel & (0x80 >> xline)) != 0)
				{
					uint8_t p = readPixel(x + xline, y + yline) != 0;
					if (p == 1)
					{
						V[0xF] = 1;
					}
//					Draw(x + xline, y + yline, p ^ 1 ? olc::WHITE : olc::BLACK);
					writePixel(x + xline, y + yline, p ^ 1);
				}
			}
		}
		
		pc += 2;
		break;
	}
	
	case 0xE:
	{
		const uint8_t op00xx = opcode & 0x00FF;
		switch (op00xx)
		{
		case 0x009E: // SKP Vx
		{
#ifdef GB_DEBUG_PRINT
			printf("SKP Vx");
#endif
			const uint8_t Vx = *getVx(opcode);
			const uint16_t filter = 0b1 << Vx;
			if (kb & filter)
			{
				printf("Key %02X pressed\n", Vx);
				pc += 2;
			}
			break;
		}

		case 0x00A1: // SKNP Vx
		{
#ifdef GB_DEBUG_PRINT
			printf("SKNP Vx");
#endif
			const uint8_t Vx = *getVx(opcode);
			const uint16_t filter = 0b1 << Vx;
			if (!(kb & filter))
			{
				printf("Key %02X unpressed\n", Vx);
				pc += 2;
			}
			break;
		}

		default:
			printf("Bad opcode : 0x%04X", opcode);
			break;
		}
		pc += 2;
		break;
	}

	case 0xF:
	{
		const uint16_t op00xx = opcode & 0x00FF;
		switch (op00xx)
		{
		case 0x0007: // LD Vx, DT
		{
#ifdef GB_DEBUG_PRINT
			printf("LD Vx, DT");
#endif
			*getVx(opcode) = DT;
			break;
		}

		case 0x000A: // LD Vx, K
		{
#ifdef GB_DEBUG_PRINT
			printf("LD Vx, K");
#endif
			if (kb > pkb)
			{
				const uint8_t key = getKey(kb);
				*getVx(opcode) = key;
				printf("Key %02X pressed\n", key);
			}
			else
			{
				pc -= 2;
			}
			break;
		}

		case 0x0015: // LD DT, Vx
#ifdef GB_DEBUG_PRINT
			printf("LD DT, Vx");
#endif
			DT = *getVx(opcode);
			break;

		case 0x0018: // LD ST, Vx
#ifdef GB_DEBUG_PRINT
			printf("LD ST, Vx");
#endif
			ST = *getVx(opcode);
			break;

		case 0x001E: // ADD I, Vx
#ifdef GB_DEBUG_PRINT
			printf("ADD I, Vx");
#endif
			I += *getVx(opcode);
			break;

		case 0x0029: // LD F, Vx
		{
#ifdef GB_DEBUG_PRINT
			printf("LD F, Vx");
#endif
			const uint8_t Vx = *getVx(opcode);
			I = FONT_START + Vx * 5;
			break;
		}

		case 0x0033: // LD B, Vx
		{
#ifdef GB_DEBUG_PRINT
			printf("LD B, Vx");
#endif
			uint8_t Vx = *getVx(opcode);
			ram[I] = Vx / 100;
			ram[I + 1] = (Vx / 10) % 10;
			ram[I + 2] = Vx % 10;
			break;
		}

		case 0x0055: // LD [I], Vx
		{
#ifdef GB_DEBUG_PRINT
			printf("LD [I], Vx");
#endif
			const uint8_t Vx = *getVx(opcode);
			for (uint8_t i = 0; i <= getValue(opcode, 1); i++)
			{
				ram[I + i] = V[i];
			}
			break;
		}

		case 0x0065: // LD Vx, [I]
		{
#ifdef GB_DEBUG_PRINT
			printf("LD Vx, [I]");
#endif
			const uint8_t Vx = *getVx(opcode);
			for (uint8_t i = 0; i <= getValue(opcode, 1); i++)
			{
				V[i] = ram[I + i];
			}
			break;
		}

		default:
			printf("Bad opcode : 0x%04X", opcode);
			break;
		}
		pc += 2;
		break;
	}

	default:
		printf("\nUnknown opcode : 0x%02X", opcode);
		running = false;
		break;
	}

#ifdef GB_DEBUG_PRINT
	printf("\n");
#endif

	//pc %= rom.size();
}

uint8_t C8Engine::getValue(const uint16_t value, const uint8_t position) const
{
	if (position >= 4)
	{
		printf("[%s] position should be between 0 and 3 (received %u)", __FUNCTION__, position);
		return 0;
	}
	const uint8_t offset = ((3 - position) * 4);
	const uint8_t result = (value & (0xF << offset)) >> offset;
	return result;
}

uint8_t* C8Engine::getVx(const uint16_t opcode)
{
	return &(V[getValue(opcode, 1)]);
}

uint8_t* C8Engine::getVy(const uint16_t opcode)
{
	return &(V[getValue(opcode, 2)]);
}

uint8_t C8Engine::getKey(const uint16_t kbValue) const
{
	return static_cast<uint8_t>(log2(kbValue));
}

//uint8_t C8Engine::readPixel(int x, int y) const
//{
//	olc::Sprite* screen = GetDrawTarget();
//	olc::Pixel pixel = screen->GetPixel(x, y);
//	return pixel.r | pixel.g | pixel.b;
//}

void C8Engine::buzz()
{
	printf("buzz !\n");
}

//void C8Engine::updateKeyboard()
//{
//	pkb = kb;
//
//	for (uint8_t i = 0; i < keys.size(); i++)
//	{
//		if (GetKey(keys[i]).bHeld)
//		{
//			kb |= 0b1 << keyMap[i];
//		}
//		else
//		{
//			kb &= ~(0b1 << keyMap[i]);
//		}
//	}
//
//	if (GetKey(olc::L).bPressed)
//	{
//		framelimit = !framelimit;
//		printf("Framelimit : %s\n", framelimit ? "ON" : "OFF");
//	}
//}

void C8Engine::updateTimers()
{
	if (DT > 0)
	{
		DT--;
	}
	if (ST > 0)
	{
		ST--;
		buzz();
	}
}

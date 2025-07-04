#define SDL_MAIN_HANDLED

#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

#include <stdio.h>

uint16_t index_register{};
uint16_t program_counter{};
uint16_t stack_pointer{0U};
uint8_t delay_timer{};
uint8_t sound_timer{};
uint8_t registers[16]{};
uint16_t stack[16]{};
uint8_t memory[4096]{};
uint32_t display[64 * 32]{};
uint8_t keys[16]{};
std::ifstream rom_file;
std::string info;
std::string rom_files[3] = {"LunarLander", "Pong", "Tetris"};

unsigned char fontset[80] =
{ 
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


class Platform
{
public:
	Platform(char const* title, int windowWidth, int windowHeight, int textureWidth, int textureHeight)
	{
		SDL_Init(SDL_INIT_VIDEO);

		window = SDL_CreateWindow(title, 0, 0, windowWidth, windowHeight, SDL_WINDOW_SHOWN);

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

		texture = SDL_CreateTexture(
			renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, textureWidth, textureHeight);
	}

	~Platform()
	{
		SDL_DestroyTexture(texture);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

    void showLeftArrow() {
        SDL_Surface * surface = SDL_LoadBMP("src/textures/left_arrow.bmp");
        SDL_Texture * lArrow = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect lArrowCrop = {0,0,10,32};
        SDL_Rect lArrowPos = {0,16,10*20,32*20};
        SDL_RenderCopy(renderer, lArrow, &lArrowCrop, &lArrowPos);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(lArrow);
    }

    void showRightArrow() {
        SDL_Surface * surface = SDL_LoadBMP("src/textures/right_arrow.bmp");
        SDL_Texture * rArrow = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect rArrowCrop = {54,0,10,32};
        SDL_Rect rArrowPos = {54*20,16,10*20,32*20};
        SDL_RenderCopy(renderer, rArrow, &rArrowCrop, &rArrowPos);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(rArrow);
    }

    void UpdateBMP(int imgIndex)
	{
        std::string path = "src/textures/" + rom_files[imgIndex] + ".bmp";
        SDL_Surface * surface = SDL_LoadBMP(&path[0]);
        SDL_Texture * img = SDL_CreateTextureFromSurface(renderer, surface);
        
		SDL_Rect img_rect;
        img_rect.x = 0;
        img_rect.y = 0;
        img_rect.w = 64*20;
        img_rect.h = 32*20;
        SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, img, nullptr, &img_rect);
        if(imgIndex > 0) {
            showLeftArrow();
        }
        if(imgIndex < sizeof(rom_files)/sizeof(rom_files[0]) - 1) {
            showRightArrow();
        }
		SDL_RenderPresent(renderer);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(img);
	}

	void Update(void const* buffer, int pitch)
	{
		SDL_UpdateTexture(texture, nullptr, buffer, pitch);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, nullptr, nullptr);
		SDL_RenderPresent(renderer);
	}

    void ClearRender() {
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

	bool ProcessInput(uint8_t* keys)
	{
		bool quit = false;

		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
				{
					quit = true;
				} break;

				case SDL_KEYDOWN:
				{
					switch (event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
						{
							quit = true;
						} break;

						case SDLK_x:
						{
							keys[0] = 1;
						} break;

						case SDLK_1:
						{
							keys[1] = 1;
						} break;

						case SDLK_2:
						{
							keys[2] = 1;
						} break;

						case SDLK_3:
						{
							keys[3] = 1;
						} break;

						case SDLK_q:
						{
							keys[4] = 1;
						} break;

						case SDLK_w:
						{
							keys[5] = 1;
						} break;

						case SDLK_e:
						{
							keys[6] = 1;
						} break;

						case SDLK_a:
						{
							keys[7] = 1;
						} break;

						case SDLK_s:
						{
							keys[8] = 1;
						} break;

						case SDLK_d:
						{
							keys[9] = 1;
						} break;

						case SDLK_z:
						{
							keys[0xA] = 1;
						} break;

						case SDLK_c:
						{
							keys[0xB] = 1;
						} break;

						case SDLK_4:
						{
							keys[0xC] = 1;
						} break;

						case SDLK_r:
						{
							keys[0xD] = 1;
						} break;

						case SDLK_f:
						{
							keys[0xE] = 1;
						} break;

						case SDLK_v:
						{
							keys[0xF] = 1;
						} break;
					}
				} break;

				case SDL_KEYUP:
				{
					switch (event.key.keysym.sym)
					{
						case SDLK_x:
						{
							keys[0] = 0;
						} break;

						case SDLK_1:
						{
							keys[1] = 0;
						} break;

						case SDLK_2:
						{
							keys[2] = 0;
						} break;

						case SDLK_3:
						{
							keys[3] = 0;
						} break;

						case SDLK_q:
						{
							keys[4] = 0;
						} break;

						case SDLK_w:
						{
							keys[5] = 0;
						} break;

						case SDLK_e:
						{
							keys[6] = 0;
						} break;

						case SDLK_a:
						{
							keys[7] = 0;
						} break;

						case SDLK_s:
						{
							keys[8] = 0;
						} break;

						case SDLK_d:
						{
							keys[9] = 0;
						} break;

						case SDLK_z:
						{
							keys[0xA] = 0;
						} break;

						case SDLK_c:
						{
							keys[0xB] = 0;
						} break;

						case SDLK_4:
						{
							keys[0xC] = 0;
						} break;

						case SDLK_r:
						{
							keys[0xD] = 0;
						} break;

						case SDLK_f:
						{
							keys[0xE] = 0;
						} break;

						case SDLK_v:
						{
							keys[0xF] = 0;
						} break;
					}
				} break;
			}
		}

		return quit;
	}

private:
	SDL_Window* window{};
	SDL_Renderer* renderer{};
	SDL_Texture* texture{};
};

void rom_selector(Platform &platform, int selectedRom) {
    platform.UpdateBMP(selectedRom);
}

void initialize_font() {
    for(int i = 0; i < 80; i++) {
        memory[0x050 + i] = fontset[i];
    }
}

void clear_memory() {
    for(int i = 0; i < sizeof(memory)/sizeof(memory[0]); i++) {
        memory[i] = 0;
    }
}

void clear_display() {
    for(int i = 0; i < sizeof(display)/sizeof(display[0]); i++) {
        display[i] = 0;
    }
}

void load_memory(int fileIndex) {
    int START_ADDRESS = 0x200U;
    rom_file.open("rom_files\\" + rom_files[fileIndex] + ".ch8", std::ios::binary);
    if(rom_file.is_open()) {       
       rom_file.seekg(0, std::ios::end);
       int size = rom_file.tellg();
       char * buffer = new char[size];

       rom_file.seekg(0, std::ios::beg);
       rom_file.read(buffer, size);
       rom_file.close();

       for(int i = 0; i < size; i++) {
            memory[START_ADDRESS + i] = buffer[i];
            printf("memory[%X] = %02X\n", START_ADDRESS + i, memory[START_ADDRESS + i]);
        }

        delete[] buffer;
        buffer = nullptr;

        program_counter = START_ADDRESS;
    }
}

void decode() {
    uint16_t oppcode = (memory[program_counter] << 8) | memory[program_counter + 1];
    uint16_t Vx = (oppcode & 0x0F00u) >> 8;
    uint16_t Vy = (oppcode & 0x00F0u) >> 4; 
    uint16_t nnn = oppcode & 0x0FFFu;
    uint16_t n = oppcode & 0x000Fu;
    uint16_t kk = oppcode & 0x00FFu;

    program_counter += 2U;

    switch (oppcode & 0xF000)
    {
        case 0x0000:
            switch (oppcode)
            {
            case 0x00E0:
                // clear display
                for(int i = 0; i < 64 * 32; i++) {
                    display[i] = 0;
                }
                break;
            
            case 0x00EE:
                /* 
                Return from a subroutine.
                The interpreter sets the program counter to the address
                at the top of the stack, then subtracts 1 from the stack pointer.
                */
                stack_pointer-=1;
                std::cout << "sp " << std::hex << stack_pointer << " pointing to " << std::hex << stack[stack_pointer] << std::endl;
                program_counter = stack[stack_pointer];
                break;
            }
        break;
        
        case 0x1000:
            /* Jump to location nnn 
            pc = nnn*/
            program_counter = nnn;
        break;
    
        case 0x2000:
            /* call subroutine at nnn
            sp++, pc -> top of stack, pc = nnn*/
            std::cout << "stack pointer " << stack_pointer << " set to " << program_counter << std::endl;
            stack[stack_pointer] = program_counter;
            stack_pointer+=1;
            std::cout << "sp pointing to " << stack_pointer << std::endl;
            program_counter = nnn;
        break;

        case 0x3000:
            if(registers[Vx] == kk) {
                program_counter += 2;
            }
        break;

        case 0x4000:
            if(registers[Vx] != kk) {
                program_counter += 2;
            }
        break;
        
        case 0x5000:
            if(registers[Vx] == registers[Vy]) {
                program_counter += 2;
            }
        break;

        case 0x6000:  
            registers[Vx] = kk;
        break;

        case 0x7000:
            registers[Vx] += kk;
        break;

        case 0x8000:
            switch (oppcode & 0x000F) {
                case 0x0000:
                    registers[Vx] = registers[Vy];
                break;
            
                case 0x0001:
                    registers[Vx] |= registers[Vy];
                break;

                case 0x0002:  
                    registers[Vx] &= registers[Vy];
                break;

                case 0x0003:
                    registers[Vx] ^= registers[Vy];
                break;

                case 0x0004: {
                    uint16_t sum = registers[Vx] + registers[Vy];

                    if(registers[Vx] > 255U) {
                        registers[15] = 1; 
                    } else {
                        registers[15] = 0;
                    }

                    registers[Vx] = sum & 0x00FF;
                    break;
                }

                case 0x0005:
                    if(registers[Vx] > registers[Vy]) {
                        registers[15] = 1;
                    } else {
                        registers[15] = 0;
                    }

                    registers[Vx] -= registers[Vy];
                break;

                case 0x0006:
                    if((registers[Vx] & 1) == 1) {
                        registers[15] = 1;
                    } else {
                        registers[15] = 0;
                    }

                    registers[Vx] /= 2;
                break;

                case 0x0007:
                    if(registers[Vy] > registers[Vx]) {
                        registers[15] = 1;
                    } else {
                        registers[15] = 0;
                    }

                    registers[Vx] = registers[Vy] - registers[Vx];
                break;

                case 0x000E:
                    if(((registers[Vx] >> 7) & 1) == 1) {
                        registers[15] = 1;
                    } else {
                        registers[15] = 0;
                    }

                    registers[Vx] *= 2;
                break;
            }
        break;

        case 0x9000:
            if(registers[Vx] != registers[Vy]) {
                program_counter += 2;
            }
        break;

        case 0xA000:
            index_register = nnn;
        break;

        case 0xB000:
            program_counter = nnn + registers[0];
        break;

        case 0xC000: {
            int randNum = std::rand()%256;
            registers[Vx] = randNum & kk;
            break;
        }

        case 0xD000: {
            
            registers[15] = 0;
            for(int r = 0; r < n; r ++) {
                for(int c = 0; c < 8; c++) {
                    uint8_t x_cord = (registers[Vx] & 63) + c;
                    uint8_t y_cord = (registers[Vy] & 31) + r;
                    uint8_t curRow = memory[index_register + r];
                    uint8_t curPix = (curRow >> (7 - c)) & 1;
                    if(display[y_cord * 64 + x_cord] != 0 && curPix != 0) {
                        registers[15] = 1;
                        display[y_cord * 64 + x_cord] = 0;
                    } else if(curPix != 0) {
                        display[y_cord * 64 + x_cord] = 0xFFFFFFFF;
                    }
                }
            }
            break;
        }

        case 0xE000:
            switch (oppcode & 0x00FF)
            {
            case 0x009E:
                if(keys[registers[Vx]] == 1) {
                    program_counter += 2;
                }

                break;
            
            case 0x00A1:
                
                if(keys[registers[Vx]] == 0) {
                    program_counter += 2;
                }
                break;
            }
        break;

        case 0xF000:
            switch (oppcode & 0x00FF)
            {
            case 0x0007:
                
                registers[Vx] = delay_timer;
                break;
            
            case 0x000A:
                
                if (keys[0])
                {
                    registers[Vx] = 0;
                }
                else if (keys[1])
                {
                    registers[Vx] = 1;
                }
                else if (keys[2])
                {
                    registers[Vx] = 2;
                }
                else if (keys[3])
                {
                    registers[Vx] = 3;
                }
                else if (keys[4])
                {
                    registers[Vx] = 4;
                }
                else if (keys[5])
                {
                    registers[Vx] = 5;
                }
                else if (keys[6])
                {
                    registers[Vx] = 6;
                }
                else if (keys[7])
                {
                    registers[Vx] = 7;
                }
                else if (keys[8])
                {
                    registers[Vx] = 8;
                }
                else if (keys[9])
                {
                    registers[Vx] = 9;
                }
                else if (keys[10])
                {
                    registers[Vx] = 10;
                }
                else if (keys[11])
                {
                    registers[Vx] = 11;
                }
                else if (keys[12])
                {
                    registers[Vx] = 12;
                }
                else if (keys[13])
                {
                    registers[Vx] = 13;
                }
                else if (keys[14])
                {
                    registers[Vx] = 14;
                }
                else if (keys[15])
                {
                    registers[Vx] = 15;
                }
                else
                {
                    program_counter -= 2;
                }

                break;

            case 0x0015:
                
                delay_timer = registers[Vx];
                break;

            case 0x0018:
                
                sound_timer = registers[Vx];
                break;

            case 0x001E:
                
                index_register += registers[Vx];
                break;

            case 0x0029:
                
                index_register = 0x0050U + (5 * registers[Vx]);
                break;

            case 0x0033: {
                
                uint8_t temp = registers[Vx];
                memory[index_register + 2] = temp % 10;
                temp /= 10;
                memory[index_register + 1] = temp % 10;
                temp /= 10;
                memory[index_register] = temp % 10;
                break;
            }

            case 0x0055: {
                
                for(int i = 0; i <= Vx; i++) {
                    memory[index_register + i] = registers[i];
                }
                break;
            }

            case 0x0065:
                
                for(int i = 0; i <= Vx; i++) {
                    registers[i] = memory[index_register + i];
                }
                break;
            }
        break;
    }
}

int main(int argc, char** argv) {
    int selectedRom = 0;
    bool romMenu = true;
    bool isInit = false;

	int videoScale = 20;
    int VIDEO_WIDTH = 64;
    int VIDEO_HEIGHT = 32;
    float waitTime = 4;
    bool nextCycle = true;

    Platform platform("CHIP-8 Emulator", VIDEO_WIDTH * videoScale, VIDEO_HEIGHT * videoScale, VIDEO_WIDTH, VIDEO_HEIGHT);

    bool quit = false;
    auto lastCycle = std::chrono::system_clock::now();
    while(!quit) {
        if(romMenu) {
            if(nextCycle) {
                nextCycle = false;
                lastCycle = std::chrono::system_clock::now();
                
                rom_selector(platform, selectedRom);

                if(keys[4] != 0 && selectedRom > 0) {
                    selectedRom--;
                    keys[4] = 0;
                } else if(keys[6] != 0 && selectedRom < sizeof(rom_files)/sizeof(rom_files[0]) - 1) {
                    selectedRom++;
                    keys[6] = 0;
                }

                if (keys[5] != 0) {
                    romMenu = false;
                }

                if(delay_timer > 0) {
                    delay_timer--;
                }
            }

            quit = platform.ProcessInput(keys);

            auto curTime = std::chrono::system_clock::now();
            auto passedTime = std::chrono::duration<float, std::chrono::milliseconds::period>(curTime - lastCycle).count();

            if(passedTime > waitTime) {
                nextCycle = true;
            }
        } else {
            if(!isInit) {
                isInit = true;
                initialize_font();
                load_memory(selectedRom);
            }

            if(nextCycle) {
                nextCycle = false;
                lastCycle = std::chrono::system_clock::now();
                decode();
                platform.Update(display, sizeof(display[0]) * 64);

                if (keys[15] != 0) {
                    clear_memory();
                    clear_display();
                    platform.ClearRender();
                    romMenu = true;
                    isInit = false;
                }

                if(delay_timer > 0) {
                    delay_timer--;
                }

                if(sound_timer > 0) {
                    sound_timer--;
                }
            }

            quit = platform.ProcessInput(keys);
            auto curTime = std::chrono::system_clock::now();
            auto passedTime = std::chrono::duration<float, std::chrono::milliseconds::period>(curTime - lastCycle).count();

            if(passedTime > waitTime) {
                nextCycle = true;
            }
        }
    }

    return 0;
}
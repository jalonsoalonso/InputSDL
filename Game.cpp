#include "Game.h"
#include <math.h>

Game::Game() {}
Game::~Game(){}

bool Game::Init()
{
	//Initialize SDL with all subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	//Create our window: title, x, y, w, h, flags
	Window = SDL_CreateWindow("Input SDL: up-down +/- speed, left-right x0.1/x10 speed", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (Window == NULL)
	{
		SDL_Log("Unable to create window: %s", SDL_GetError());
		return false;
	}
	//Create a 2D rendering context for a window: window, device index, flags
	Renderer = SDL_CreateRenderer(Window, -1, 0);
	if (Renderer == NULL)
	{
		SDL_Log("Unable to create rendering context: %s", SDL_GetError());
		return false;
	}
	//Initialize keys array
	for (int i = 0; i < MAX_KEYS; ++i)
		keys[i] = KEY_IDLE;

	//Init variables
	alpha = 0.0f;
	speed = 2.0f;

	return true;
}
void Game::Release()
{
	//Clean up all SDL initialized subsystems
	SDL_Quit();
}
bool Game::Input()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)	return false;
	}

	SDL_PumpEvents();
	const Uint8* keyboard = SDL_GetKeyboardState(NULL);
	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keyboard[i])
			keys[i] = (keys[i] == KEY_IDLE) ? KEY_DOWN : KEY_REPEAT;
		else
			keys[i] = (keys[i] == KEY_REPEAT || keys[i] == KEY_DOWN) ? KEY_UP : KEY_IDLE;
	}
	
	return true;
}
bool Game::Update()
{
	//Read Input
	if (!Input())	return true;

	//Process Input
	if (keys[SDL_SCANCODE_ESCAPE] == KEY_DOWN)	return true;
	if (keys[SDL_SCANCODE_UP] == KEY_REPEAT)	speed += 0.1f;
	if (keys[SDL_SCANCODE_DOWN] == KEY_REPEAT)	speed -= 0.1f;
	if (keys[SDL_SCANCODE_LEFT] == KEY_DOWN)	speed -= 1.0f;
	if (keys[SDL_SCANCODE_RIGHT] == KEY_DOWN)	speed += 1.0f;
	if (speed > 10.0f)		speed = 10.0f;
	else if (speed < 0.0f)	speed = 0.0f;

	//Logic
	alpha += speed * (M_PI / 180.0f);

	return false;
}
void Game::Draw()
{
	//Set the color used for drawing operations
	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
	//Clear rendering target
	SDL_RenderClear(Renderer);

	//Draw GUI
	SDL_Rect rc; 
	const int h = speed*10,
	          w = 4,
	          cx = WINDOW_WIDTH >> 1,
	          cy = WINDOW_HEIGHT >> 1;
	
	rc = { 10, 10, 20+2*w, 100+2*w }; 
	SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(Renderer, &rc);
	
	rc = { 10+w, 10+100+w-h, 20, h };
	SDL_SetRenderDrawColor(Renderer, 0, 192, 0, 255);
	SDL_RenderFillRect(Renderer, &rc);

	//Draw radar
	SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
	SDL_RenderDrawLine(Renderer, cx, cy, cx + cy * cos(alpha), cy - cy * sin(alpha));

	//Update screen
	SDL_RenderPresent(Renderer);

	SDL_Delay(10);	// 1000/10 = 100 fps max
}
// Victoria Wilke - 2004466
// CMP202 Project
// Lab 5 - Taskbased was taken as a starting point for this project

#include <iostream>
#include <fstream>
#include <string>
#include <SDL.h>
#include <vector>
#include "farm.h"
#include "task.h"
#include "mandelbrottask.h"
#include "mandelbrotinfo.h"
#include "bwmandelbrottask.h"

// Import things we need from the standard library
using std::cout;
using std::endl;
using std::mutex;
using std::vector;

// The size of the image to generate.
const int WIDTH = 1120;
const int HEIGHT = 700;

SDL_Window* window;		// main window we'll be rendering to
SDL_Event ev;		// event handler
SDL_Texture* texture;		// texture to write buffer data to
SDL_Renderer* ren;		// window renderer
mutex textureMutex;		// only one thread updating screen
const int MAX_ITERATIONS = 500;		// number of times to iterate before assuming point not in set
MandelbrotData MandelData;		// describes what area of the mandelbrot should be drawn
vector<MandelbrotInfo*> Mandelbrot;		// vector contains info about mandelbrot
Farm m; // instance of farm class
int numThreads = 12; // change threads number
vector<bool> setKey; // vector holds the status of each key

typedef std::chrono::steady_clock theClock; // timer

// mutex condMutex;
// std::condition_variable condVar;

// SDL Learning: https://lazyfoo.net/tutorials/SDL/index.php#Hello%20SDL
void createWindow()
{
	// initialise SDL
	if (SDL_Init(SDL_INIT_EVERYTHING)) // reference: https://wiki.libsdl.org/SDL_Init
	{		
		cout << "SDL couldn't initialise! Error: " << SDL_GetError() << endl;
		SDL_Delay(3000); // waits 3 seconds (3000ms)
		exit(1); // shows error in console because exit wasn't 0
	}

	// create window
	window = SDL_CreateWindow("Mandelbrot", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);		// window is implicitly shown anyway
	if (!window) // if window wasn't created
	{
		cout << "Window could not be created! Error: " << SDL_GetError() << endl;
		SDL_Delay(3000);
		exit(1);
	}

	// create renderer for window
	ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!ren) // if renderer wasn't created
	{
		cout << "Renderer could not  be created! Error: " << SDL_GetError() << endl;
		SDL_Delay(3000);
		exit(1);
	}

	// create texture
	texture = SDL_CreateTexture(ren, SDL_GetWindowPixelFormat(window),
		SDL_TEXTUREACCESS_STREAMING, WIDTH,	HEIGHT);
	if (!texture)
	{
		cout << "Unable to create texture! Error: " << SDL_GetError();
		SDL_Delay(3000);
		exit(1);
	}
	// clear screen
	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255); // white
	SDL_RenderClear(ren);
	SDL_RenderPresent(ren); //shows window
}

void setKeys(const SDL_Event& ev)
{
	// use number keys to change how many threads are running
	switch (ev.key.keysym.sym)
	{
	case SDLK_LSHIFT:
		setKey[0] = true;
		break;
	case  SDLK_1:
		setKey[1] = true;
		break;
	case SDLK_2:
		setKey[2] = true;
		break;
	case SDLK_3:
		setKey[3] = true;
		break;
	case SDLK_4:
		setKey[4] = true;
		break;
	case  SDLK_5:
		setKey[5] = true;
		break;
	case SDLK_6:
		setKey[6] = true;
		break;
	case SDLK_7:
		setKey[7] = true;
		break;
	case SDLK_8:
		setKey[8] = true;
		break;
	case SDLK_9:
		setKey[9] = true;
		break;
	case SDLK_LCTRL:
		setKey[10] = true;
		break;
	default:
		break;
	}
}

void varyThreads(int numThreads)
{
	m.createWorkers(numThreads);
	cout << numThreads << " threads running" << endl;
}

void addingTasks()
{
	for (long j = 0; j < Mandelbrot.size(); j++) { // mandelbrot size is the amount of lines
		m.add_task(new MandelbrotTask(Mandelbrot[j])); // adds line as a task
	}
}

void addingBWTasks()
{
	for (long j = 0; j < Mandelbrot.size(); j++) { // mandelbrot size is the amount of lines
		m.add_task(new BWMandelbrotTask(Mandelbrot[j])); // adds line as a task
	}
}

void drawLine()
{
	// drawing each line
	for (long i = 0; i < Mandelbrot.size(); i++) {
		Mandelbrot[i]->draw(texture, &textureMutex, i);
	}
	//condVar.notify_one();
}

void handleInput()
{
	/*std::unique_lock<mutex> condLock(condMutex);
	condVar.wait(condLock); */
	if (setKey[0]) { addingBWTasks(); }
	if (setKey[1]) { varyThreads(1); }
	if (setKey[2]) { varyThreads(2); }
	if (setKey[3]) { varyThreads(3); }
	if (setKey[4]) { varyThreads(4); }
	if (setKey[5]) { varyThreads(5); }
	if (setKey[6]) { varyThreads(6); }
	if (setKey[7]) { varyThreads(7); }
	if (setKey[8]) { varyThreads(8); }
	if (setKey[9]) { varyThreads(9); }
	if (setKey[10]) { addingTasks(); }
}

int main(int argc, char* argv[])
{
	// set up lines
	for (int i = 0; i < HEIGHT; i++) {
		Mandelbrot.push_back(new MandelbrotInfo(i));
	}

	cout << "Starting task farm...\n";
	addingTasks();

	m.createWorkers(numThreads);

	// sdl code to create base window
	createWindow();
	
	// free space in array
	while (setKey.size() < 64) {
		setKey.push_back(false);
	}

	bool quit = false; // used to flag loop
	bool timings = false;

	while (!quit) // while app is running
	{
		while (SDL_PollEvent(&ev)) // handle events
		{
			if (ev.type == SDL_QUIT) // if user quits
			{ quit = true; }
			else if (ev.type == SDL_KEYDOWN) { // if user uses keyboard
				if (ev.key.keysym.sym == SDLK_ESCAPE) { quit = true; } // lets user use escape to quit
				setKeys(ev); // this sets keys to true (so they only happen once)
				timings = true; // if key has been changed time
			}
		}
		theClock::time_point start = theClock::now();

		handleInput();
		drawLine();
		//thread t1 (handleInput);
		//thread t2 (drawLine);

		//t1.join();
		//t2.join();

		// screen texture to window
		textureMutex.lock();
		{
			SDL_RenderCopy(ren, texture, NULL, NULL);
			SDL_RenderPresent(ren);
		}
		textureMutex.unlock();
		
		std::fill(setKey.begin(), setKey.end(), false);

		if (timings)
		{
			theClock::time_point end = theClock::now();
			auto time_taken = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
			cout << "Computing the Mandelbrot set took " << time_taken << " ns." << endl;
			timings = false;
		}
	}
	return 0;
}
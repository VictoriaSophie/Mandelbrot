#ifndef MANDELBROTINFO_H
#define MANDELBROTINFO_H

#include <mutex>
#include <atomic>
#include <sdl.h> // simple directmedia layer to make mandelbrot to graphics thing

using std::mutex;

// struct containing info we need to draw mandelbrot
struct MandelbrotData {
	double left = -2.0;
	double right = 1.0;
	double top = 1.125;
	double bottom = -1.125;
};

extern MandelbrotData MandelData; // other class to access
extern const int WIDTH;	// width from taskbased

class MandelbrotInfo
{
public:
	MandelbrotInfo(int height);
	~MandelbrotInfo(); //destructor

	// drawing line 
	void draw(SDL_Texture* ren, mutex* textureMutex_, int position);
	mutex textureMutex_;

	Uint32* data_; // array for pixels
	int yend_;
	

};

#endif
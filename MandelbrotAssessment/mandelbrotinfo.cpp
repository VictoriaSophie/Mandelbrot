#include "mandelbrotinfo.h"


MandelbrotInfo::MandelbrotInfo(int height)
{
	yend_ = height;

	data_ = new Uint32[WIDTH]();
}

// destructor
MandelbrotInfo::~MandelbrotInfo()
{
	delete[] data_;
	data_ = nullptr;
}

void MandelbrotInfo::draw(SDL_Texture* tex, mutex* textureMutex_, int position)
{
	textureMutex_->lock();
	SDL_Rect drawArea{ 0, position, WIDTH, 1 };
	SDL_UpdateTexture(tex, &drawArea, data_, sizeof(Uint32) * WIDTH);
	textureMutex_->unlock();
}

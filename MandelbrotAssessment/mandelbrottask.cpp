#include "mandelbrottask.h"
#include "mandelbrotinfo.h"

#include <complex>
#include <iostream>

using std::complex;
using std::mutex;
using std::unique_lock;

MandelbrotTask::MandelbrotTask(MandelbrotInfo* line)
{
	line_ = line;
}

void MandelbrotTask::run() {

	// required data
	int y = line_->yend_;
	double left = MandelData.left;
	double right = MandelData.right;
	double top = MandelData.top;
	double bottom = MandelData.bottom;

	for (int x = 0; x < WIDTH; ++x)
	{
		// Work out the point in the complex plane that
		// corresponds to this pixel in the output image.
		complex<double> c(left + (x * (right - left) / WIDTH),
			top + (y * (bottom - top) / HEIGHT));

		complex<double> z(0.0, 0.0);

		int iterations = 0;
		while (abs(z) < 2.0 && iterations < MAX_ITERATIONS)
		{
			z = (z * z) + c;

			++iterations;
		}

		if (iterations >= MAX_ITERATIONS) // this point is in the Mandelbrot set
		{
			unique_lock<mutex> lineLock(line_->textureMutex_);
			if (y < 100 && y >= 1)
				line_->data_[x] = 0xFF0000; // red
			else if (y < 180 && y >= 100)
				line_->data_[x] = 0xFFA500; // orange
			else if (y < 250 && y >= 180)
				line_->data_[x] = 0xFFFF00; // yellow
			else if (y < 320 && y >= 250)
				line_->data_[x] = 0x90ee90; // light green
			else if (y < 400 && y >= 320)
				line_->data_[x] = 0x006400; // dark green
			else if (y < 480 && y >= 400)
				line_->data_[x] = 0x00008B; // dark blue
			else if (y < 580 && y >= 480)
				line_->data_[x] = 0x800080; // purple
			else if (y < 700 && y >= 580)
				line_->data_[x] = 0xFFC0CB; // pink 
			else	// just in case
				line_->data_[x] = 0xFFFFFF; // white
		}
		else
		{
			unique_lock < mutex> lineLock(line_->textureMutex_);
			line_->data_[x] = 0x000000; // black
		}
	}
}
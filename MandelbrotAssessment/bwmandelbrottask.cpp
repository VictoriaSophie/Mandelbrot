#include "bwmandelbrottask.h"
#include "mandelbrotinfo.h"

#include <complex>
#include <iostream>

using std::complex;
using std::mutex;
using std::unique_lock;

BWMandelbrotTask::BWMandelbrotTask(MandelbrotInfo* line)
{
	line_ = line;
}

void BWMandelbrotTask::run() {

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

		if (iterations >= MAX_ITERATIONS) // This point is in the Mandelbrot set
		{
			unique_lock<mutex> lineLock(line_->textureMutex_);
			line_->data_[x] = 0xFFFFFF; // white
		}
		else
		{
			unique_lock < mutex> lineLock(line_->textureMutex_);
			line_->data_[x] = 0x000000; // black
		}
	}
}
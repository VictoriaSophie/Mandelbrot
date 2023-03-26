// Victoria Wilkie - 2004466
// CMP 202 Unit 2 Project

// include things
#include <thread>
#include <iostream> // lets u use std::
#include <fstream> // ofstream etc
#include <complex>

// imported things from std library
using std::cout;
using std::thread;
using std::this_thread::sleep_for;
using std::chrono::seconds;
using std::ofstream;
using std::endl;
using std::complex;

// Things used to set up mandelbrot has been copied from Lab 1
// The size of the image to generate.
const int WIDTH = 1920;
const int HEIGHT = 1200;

// The number of times to iterate before we assume that a point isn't in the
// Mandelbrot set.
// (You may need to turn this up if you zoom further into the set.)
const int MAX_ITERATIONS = 500;

// The image data.
// Each pixel is represented as 0xRRGGBB.
uint32_t image[HEIGHT][WIDTH];


// Write the image to a TGA file with the given name.
// Format specification: http://www.gamers.org/dEngine/quake3/TGA.txt
void write_tga(const char* filename)
{
	ofstream outfile(filename, ofstream::binary);

	uint8_t header[18] = {
		0, // no image ID
		0, // no colour map
		2, // uncompressed 24-bit image
		0, 0, 0, 0, 0, // empty colour map specification
		0, 0, // X origin
		0, 0, // Y origin
		WIDTH & 0xFF, (WIDTH >> 8) & 0xFF, // width
		HEIGHT & 0xFF, (HEIGHT >> 8) & 0xFF, // height
		24, // bits per pixel
		0, // image descriptor
	};
	outfile.write((const char*)header, 18);

	for (int y = 0; y < HEIGHT; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			uint8_t pixel[3] = {
				image[y][x] & 0xFF, // blue channel
				(image[y][x] >> 8) & 0xFF, // green channel
				(image[y][x] >> 16) & 0xFF, // red channel
			};
			outfile.write((const char*)pixel, 3);
		}
	}

	outfile.close();
	if (!outfile)
	{
		// An error has occurred at some point since we opened the file.
		cout << "Error writing to " << filename << endl;
		exit(1);
	}
}

// defining things

// function for user input

// timings functions

// thread function for user input 1

void colourThreadFunc(double left, double right, double top, double bottom, unsigned ystart, unsigned yend)
{
	for (unsigned y = ystart; y < yend; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			// Work out the point in the complex plane that
			// corresponds to this pixel in the output image.
			complex<double> c(left + (x * (right - left) / WIDTH),
				top + (y * (bottom - top) / HEIGHT));

			// Start off z at (0, 0).
			complex<double> z(0.0, 0.0);

			// Iterate z = z^2 + c until z moves more than 2 units
			// away from (0, 0), or we've iterated too many times.
			int iterations = 0;
			while (abs(z) < 2.0 && iterations < MAX_ITERATIONS)
			{
				z = (z * z) + c;

				++iterations;
			}

			if (iterations == MAX_ITERATIONS) // This point is in the Mandelbrot set.// z didn't escape from the circle.
			{
				if (y < 140 && y >= 1)
					image[y][x] = 0xFF0000; // red
				else if (y < 270 && y >= 140)
					image[y][x] = 0xFFA500; // orange
				else if (y < 400 && y >= 270)
					image[y][x] = 0xFFFF00; // yellow
				else if (y < 540 && y >= 400)
					image[y][x] = 0x90ee90; // light green
				else if (y < 670 && y >= 540)
					image[y][x] = 0x006400; // dark green
				else if (y < 800 && y >= 670)
					image[y][x] = 0x00008B; // dark blue
				else if (y < 940 && y >= 800)
					image[y][x] = 0x800080; // purple
				else if (y < 1080 && y >= 940)
					image[y][x] = 0xFFC0CB; // pink 
				else
					image[y][x] = 0x000000; // black
			}
			else
			{
				// z escaped within less than MAX_ITERATIONS
				// iterations. This point isn't in the set.
				image[y][x] = 0xFFFFFF; // white
			}
		}
	}
}

// second thread function for black and white image
void bAndWThreadFunc(double left, double right, double top, double bottom)
{
	for (int y = 0; y < HEIGHT; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			// Work out the point in the complex plane that
			// corresponds to this pixel in the output image.
			complex<double> c(left + (x * (right - left) / WIDTH),
				top + (y * (bottom - top) / HEIGHT));

			// Start off z at (0, 0).
			complex<double> z(0.0, 0.0);

			// Iterate z = z^2 + c until z moves more than 2 units
			// away from (0, 0), or we've iterated too many times.
			int iterations = 0;
			while (abs(z) < 2.0 && iterations < MAX_ITERATIONS)
			{
				z = (z * z) + c;

				++iterations;
			}

			if (iterations == MAX_ITERATIONS)
			{
				// z didn't escape from the circle.
				// This point is in the Mandelbrot set.
				image[y][x] = 0x000000; // black
			}
			else
			{
				// z escaped within less than MAX_ITERATIONS
				// iterations. This point isn't in the set.
				image[y][x] = 0xFFFFFF; // white
			}
		}
	}
}

/*
// second thread function for user input 2
void threadTwoFunc(int num)
{
	sleep_for(seconds(1));
	cout << "I am thread two\n";
	cout << num;
} */

// mandelbrot functions

int main(int argc, char* argv[])
{
	cout << "Please wait..." << endl;

	// This shows the whole set.
	thread threadOne(colourThreadFunc, -2.0, 1.0, 1.125, -1.125, 0, 216);
	thread threadTwo(colourThreadFunc, -2.0, 1.0, 1.125, -1.125, 216, 432);
	thread threadThree(colourThreadFunc, -2.0, 1.0, 1.125, -1.125, 432, 648);
	thread threadFour(colourThreadFunc, -2.0, 1.0, 1.125, -1.125, 648, 864);
	thread threadFive(colourThreadFunc, -2.0, 1.0, 1.125, -1.125, 864, 1080);

	threadOne.join();
	threadTwo.join();
	threadThree.join();
	threadFour.join();
	threadFive.join();

	write_tga("output.tga");

	cout << "dunzo" << endl;
	// output to tell user + ask

	// user input function

	// if / else if depending on what user inputted
	// compute mandelbrot

	// tell user things

	// write to output

	return 0;
}
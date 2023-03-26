#ifndef MANDELBROTTASK_H
#define MANDELBROTTASK_H

#include "task.h"
#include <string>

// data from info class
class MandelbrotInfo;

// data from main etc
extern const int WIDTH;
extern const int HEIGHT;
extern const int MAX_ITERATIONS;


class MandelbrotTask : public Task
{
public:
	MandelbrotTask(MandelbrotInfo* line);

	void run() override;


private:
	MandelbrotInfo* line_;

};

#endif
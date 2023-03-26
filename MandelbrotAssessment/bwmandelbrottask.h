#ifndef BWMANDELBROTTASK_H
#define BWMANDELBROTTASK_H

#include "task.h"
#include <string>

// data from info class
class MandelbrotInfo;

// data from main etc
extern const int WIDTH;
extern const int HEIGHT;
extern const int MAX_ITERATIONS;


class BWMandelbrotTask : public Task
{
public:
	BWMandelbrotTask(MandelbrotInfo* line);

	void run() override;


private:
	MandelbrotInfo* line_;

};

#endif
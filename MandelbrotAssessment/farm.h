#ifndef FARM_H
#define FARM_H

#include "task.h"
#include <queue>
#include <mutex>
#include <thread>

using std::queue;
using std::mutex;
using std::vector;
using std::thread;
using std::atomic_bool;

class Farm {
public:
	Farm() { finishedWork = false; }
	~Farm() {}

	void add_task(Task* task);

	void createWorkers(int numThreads); // contains loops to create worker threads
	
	void joinWorkers();
	
	void run();

private:
	
	void createWorker(); // actually creates the worker threads
	
	mutex taskMutex; // mutex to block threads completing the same task
	
	queue<Task*> taskQueue; // queue containing the tasks to be completed

	vector<thread*> threads;

	atomic_bool finishedWork;
};

#endif


#include "farm.h"
#include <queue>
#include <mutex>
#include <thread>
#include <functional>

using std::queue;
using std::mutex;
using std::vector;
using std::thread;

void Farm::add_task(Task* task)
{
	// mutex lock
	taskMutex.lock();

	// add to queue
	taskQueue.push(task);

	//unlock mutex
	taskMutex.unlock();
}

void Farm::createWorkers(int numThreads)
{
	joinWorkers(); // close running threads
	threads.clear();

	//creating threads
	for (int i = 0; i < numThreads; i++)
	{
		createWorker();
	}
}

void Farm::createWorker()
{
	finishedWork = false;
	threads.push_back(new thread(std::mem_fun(&Farm::run), this)); // in brackets function to be done then any arguments
}

void Farm::joinWorkers()
{
	finishedWork = true;
	for (auto& i : threads)
	{
		i->join();
	}
}

void Farm::run()
{
	while (!finishedWork)
	{
		taskMutex.lock();
		if (!taskQueue.empty())
		{
			Task* t = taskQueue.front();

			taskQueue.pop(); // remove from queue

			taskMutex.unlock();

			t->run();	// run task

			delete t; // delete task
		}
		else
		{
			taskMutex.unlock();

			std::this_thread::sleep_for(std::chrono::milliseconds(40)); // busy wait
		}
	}
}
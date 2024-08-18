#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <uv.h>
#include <vector>
#include <iostream>
#include "JobQueue.h"
#include "Expression.h"
#include "TimeoutItem.h"

using namespace std;

class EventLoop
{
public:
	static void SetupEventLoop();
	static void ClearTimer(int identifier);
	static int SetupTimer(Expression* exp);
	static void SetupPromise(Expression* exp);
	static void StartTimer(int delay, bool repeate);
	static bool WasCompleted();
	static void StartPromise();
	static void SetupJobContextToStack(uv_timer_t* timer);
	static void SetupJobPromiseContextToStack(uv_timer_t* timer);
	static void Run();
	static void RunAsync();

	static uv_loop_t* eventloop;
	static uv_thread_t loopThread;
	static vector<TimeoutItem*> timerList;
	static vector<uv_timer_t*> promiseList;
	static int timerCount;
};

#endif
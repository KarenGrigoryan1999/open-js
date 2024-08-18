#pragma once
#include <queue>
#include <iostream>
#include "Expression.h"
#include "CallExpression.h"
#include "Program.h"

using namespace std;

class JobQueue
{
public:
	static queue<Expression*> PromiseJobsQueue;
	static vector<Expression*> TimerJobsQueue;

	static void AddToPromiseQueue(Expression* context);
	static void AddToTimerQueue(Expression* context);

	static bool IsPromiseJobsQueueEmpty();
	static bool IsTimerJobsQueueEmpty();

	static Expression* PrepareTimerJobsTask();
	static Expression* PreparePromiseJobsTask();
};

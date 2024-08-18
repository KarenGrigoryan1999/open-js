#include "JobQueue.h"

using namespace std;

queue<Expression*> JobQueue::PromiseJobsQueue;
vector<Expression*> JobQueue::TimerJobsQueue;

void JobQueue::AddToPromiseQueue(Expression* context) {
	JobQueue::PromiseJobsQueue.push(context);
	Program::Run();
}

void JobQueue::AddToTimerQueue(Expression* context) {
	JobQueue::TimerJobsQueue.push_back(context);
	Program::Run();
}

bool JobQueue::IsPromiseJobsQueueEmpty() {
	if (JobQueue::PromiseJobsQueue.size() == 0) return true;
	return false;
}

bool JobQueue::IsTimerJobsQueueEmpty() {
	if (JobQueue::TimerJobsQueue.size() == 0) return true;
	return false;
}

Expression* JobQueue::PrepareTimerJobsTask() {
	Expression* currentTask = JobQueue::TimerJobsQueue.at(0);

	JobQueue::TimerJobsQueue.erase(JobQueue::TimerJobsQueue.begin());

	return currentTask;
}

Expression* JobQueue::PreparePromiseJobsTask() {
	Expression* currentTask = JobQueue::PromiseJobsQueue.front();

	JobQueue::PromiseJobsQueue.pop();

	return currentTask;
}
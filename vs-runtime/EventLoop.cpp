#include "EventLoop.h"

uv_loop_t* EventLoop::eventloop = NULL;
uv_thread_t EventLoop::loopThread;
vector<TimeoutItem*> EventLoop::timerList;
vector<uv_timer_t*> EventLoop::promiseList;
int EventLoop::timerCount = 0;

void EventLoop::SetupEventLoop() {
	uv_loop_t* loop = uv_loop_new();
	EventLoop::eventloop = loop;
	uv_loop_init(EventLoop::eventloop);
}

bool EventLoop::WasCompleted() {
	if (EventLoop::timerList.size() == 0 && EventLoop::promiseList.size() == 0) return true;

	return false;
}

void EventLoop::SetupJobContextToStack(uv_timer_t* timer) {

	for (int i = 0; i < EventLoop::timerList.size(); i++) {
		if (EventLoop::timerList.at(i)->timer->data == timer->data) {
			EventLoop::timerList.erase(EventLoop::timerList.begin()+i);
		}
	}

	JobQueue::AddToTimerQueue((Expression*)timer->data);
}

void EventLoop::SetupJobPromiseContextToStack(uv_timer_t* timer) {
	for (int i = 0; i < EventLoop::promiseList.size(); i++) {
		if (EventLoop::promiseList.at(i)->data == timer->data) {
			EventLoop::promiseList.erase(EventLoop::promiseList.begin() + i);
		}
	}

	JobQueue::AddToPromiseQueue((Expression*)timer->data);
}

void EventLoop::ClearTimer(int identifier) {
	for (int i = 0; i < EventLoop::timerList.size(); i++) {
		if (EventLoop::timerList.at(i)->identifier == identifier) {
			uv_timer_stop(EventLoop::timerList.at(i)->timer);
			EventLoop::timerList.erase(EventLoop::timerList.begin() + i);
		}
	}
}

int EventLoop::SetupTimer(Expression* exp) {
	EventLoop::timerCount += 1;
	uv_timer_t* timer = new uv_timer_t;
	TimeoutItem* item = new TimeoutItem{timer, EventLoop::timerCount };
	EventLoop::timerList.push_back(item);
	timer->data = exp;
	uv_timer_init(EventLoop::eventloop, EventLoop::timerList.at(EventLoop::timerList.size() - 1)->timer);

	return EventLoop::timerCount;
}

void EventLoop::SetupPromise(Expression* exp) {
	uv_timer_t* timer = new uv_timer_t;
	EventLoop::promiseList.push_back(timer);
	timer->data = exp;

	uv_timer_init(EventLoop::eventloop, EventLoop::promiseList.at(EventLoop::promiseList.size() - 1));
}

void EventLoop::StartTimer(int delay, bool repeate) {
	uv_timer_start(EventLoop::timerList.at(EventLoop::timerList.size() - 1)->timer, EventLoop::SetupJobContextToStack, !repeate ? delay + 10 : 0, repeate ? delay + 10 : 0);
}

void EventLoop::StartPromise() {
	uv_timer_start(EventLoop::promiseList.at(EventLoop::promiseList.size() - 1), EventLoop::SetupJobPromiseContextToStack, 0, 0);
}

void EventLoop::Run() {
	uv_run(EventLoop::eventloop, UV_RUN_DEFAULT);
}

void EventLoop::RunAsync() {
	uv_thread_join(&EventLoop::loopThread);
}

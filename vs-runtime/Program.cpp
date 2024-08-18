#include "Program.h"

void Program::Run() {
    {
        Node* task;
        //ѕока очередь промисов не пуста, выт€гиваем по очереди и выполн€ем
        while (!JobQueue::IsPromiseJobsQueueEmpty()) {
            task = JobQueue::PreparePromiseJobsTask();
            task->eval();
        }
        if (!JobQueue::IsTimerJobsQueueEmpty()) {//≈сли очередь таймеров не пуста, то берем одну таску из очереди таймеров и выполн€ем
            task = JobQueue::PrepareTimerJobsTask();//готовим таску к выполнению
            task->eval();
        }
    };
}

void Program::Run(Node* task) {
    task->eval();
}
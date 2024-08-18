#include "Program.h"

void Program::Run() {
    {
        Node* task;
        //���� ������� �������� �� �����, ���������� �� ������� � ���������
        while (!JobQueue::IsPromiseJobsQueueEmpty()) {
            task = JobQueue::PreparePromiseJobsTask();
            task->eval();
        }
        if (!JobQueue::IsTimerJobsQueueEmpty()) {//���� ������� �������� �� �����, �� ����� ���� ����� �� ������� �������� � ���������
            task = JobQueue::PrepareTimerJobsTask();//������� ����� � ����������
            task->eval();
        }
    };
}

void Program::Run(Node* task) {
    task->eval();
}
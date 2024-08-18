#pragma once
#include <uv.h>
#include <vector>
#include "JobQueue.h"
#include "Node.h"

using namespace std;

class Program
{
public:
	static void Run(Node* task);
	static void Run();
};

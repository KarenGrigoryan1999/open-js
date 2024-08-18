#pragma once
#include <uv.h>

struct TimeoutItem {
	uv_timer_t* timer;
	int identifier;
};
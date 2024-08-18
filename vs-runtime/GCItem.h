#pragma once
#include "CollectableEntity.h"

struct GCItem {
	CollectableEntity* entity;
	bool wasMarked;
};
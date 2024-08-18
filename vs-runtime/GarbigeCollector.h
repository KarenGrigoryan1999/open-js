#ifndef GC_H
#define GC_H

#include <vector>
#include "CollectableEntity.h"
#include "EcmaContextStack.h"
#include "GCItem.h"

#define MAX_HEAP_SIZE 10912

using namespace std;

class GarbigeCollector
{
public:
	GarbigeCollector();
	void RegisterObj(CollectableEntity* entity);
	void FindAndMark(CollectableEntity* entity);
	void RemoveColectedElements();
	static GarbigeCollector* Instance();
private:
	int heap_size_;
	vector<GCItem>* collectable_entities_;
	void StartCollecting();
	static GarbigeCollector* instance_;
};

#endif
#ifndef COLLECTABLE_ENTITY_H
#define COLLECTABLE_ENTITY_H

class CollectableEntity
{
public:
	CollectableEntity();
	void registerInHeap();
	virtual int get_data_size();
	virtual int get_entity_type();
	virtual void trace();
};

#endif
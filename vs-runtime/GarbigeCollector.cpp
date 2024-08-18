#include "GarbigeCollector.h"
#include <iostream>
#include "ObjectExpression.h"
#include "TDZValue.h"
#include "UndefinedType.h"
#include "BoolType.h"
#include "NullType.h"
#include "NumberType.h"
#include "StringType.h"

GarbigeCollector* GarbigeCollector::instance_ = nullptr;

GarbigeCollector* GarbigeCollector::Instance() {
	if (!GarbigeCollector::instance_) {
		GarbigeCollector::instance_ = new GarbigeCollector();
	}

	return GarbigeCollector::instance_;
}

GarbigeCollector::GarbigeCollector() {
	this->collectable_entities_ = new vector<GCItem>();
	this->heap_size_ = 0;
}

void GarbigeCollector::RegisterObj(CollectableEntity* entity) {
	this->collectable_entities_->push_back({entity, false});

	this->heap_size_ += entity->get_data_size();

	if (this->heap_size_ > MAX_HEAP_SIZE) {
		this->StartCollecting();
	}
}

void GarbigeCollector::FindAndMark(CollectableEntity* entity) {
	for (auto it = this->collectable_entities_->begin(); it != this->collectable_entities_->end(); ++it)
	{
		if (it->entity == entity) {
			it->wasMarked = true;
		}
	}
}

void GarbigeCollector::RemoveColectedElements() {
	vector<int> elementsForRemoving;
	int i = 0;
	for (auto it = this->collectable_entities_->begin(); it != this->collectable_entities_->end(); ++it) {
		if (!it->wasMarked) {
			CollectableEntity* entity = it->entity;

			it->entity = nullptr;
			elementsForRemoving.push_back(i);
			this->heap_size_ -= entity->get_data_size();

			if (entity->get_entity_type() == 1) {
				BaseValue* val = (BaseValue*)(entity);
				delete val;
			}
			else if (entity->get_entity_type() == 3) {
				ObjectExpression* val = (ObjectExpression*)(entity);
				delete val;
			}
			else if (entity->get_entity_type() == 4) {
				TDZValue* val = (TDZValue*)(entity);
				this->heap_size_ -= val->get_data_size();
				delete val;
			}
			else if (entity->get_entity_type() == 5) {
				UndefinedType* val = (UndefinedType*)(entity);
				delete val;
			}
			else if (entity->get_entity_type() == 6) {
				BoolType* val = (BoolType*)(entity);
				delete val;
			}
			else if (entity->get_entity_type() == 7) {
				NullType* val = (NullType*)(entity);
				delete val;
			}
			else if (entity->get_entity_type() == 8) {
				NumberType* val = (NumberType*)(entity);
				delete val;
			}
			else if (entity->get_entity_type() == 10) {
				StringType* val = (StringType*)(entity);
				delete val;
			}
		}
		else {
			it->wasMarked = false;
		}
		i++;
	}

	for (int i = elementsForRemoving.size() - 1; i >= 0; --i) {
		this->collectable_entities_->erase(this->collectable_entities_->begin() + elementsForRemoving.at(i));
	}
}

// Метод, который начинает сборку мусора
void GarbigeCollector::StartCollecting() {
	EcmaExecutionContext* executionContext = EcmaContextStack::Instance()->top();
	int stack_size = EcmaContextStack::Instance()->size();

	map<string, IdentifierCollection::IdentifierValue> *current = &IdentifierCollection::global_identifiers;

	deque<EcmaExecutionContext*>* q;
	q = (std::deque<EcmaExecutionContext*>*) & *EcmaContextStack::Instance()->GetContextStack();

	//Проходимся по стеку
	for (int i = 0; i <= stack_size; i++) {
		if(i > 0) current = &((*q)[i-1]->GetEnvironmentRecord()->records->identifiers);
		for (auto it = current->begin(); it != current->end(); ++it)
		{
			auto variable = it->second.value;
			variable->trace();
		}
	}

	//Удаляем не маркнутые элементы
	GarbigeCollector::RemoveColectedElements();
}
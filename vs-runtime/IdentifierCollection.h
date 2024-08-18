#ifndef IDENTIFIER_COLLECTION_H
#define IDENTIFIER_COLLECTION_H

#include <map>
#include <string>
#include <stack>
#include "BaseValue.h"

using namespace std;

class IdentifierCollection
{
public:
	struct IdentifierValue {
		BaseValue* value;
		bool is_const;
	};
	map<string, IdentifierValue> identifiers;
	static map<string, IdentifierValue> global_identifiers;
	BaseValue* GetLocal(string name);
	static BaseValue* GetGlobal(string name);
	void SetLocal(string name, BaseValue* data, bool is_const);
	static void SetGlobal(string name, BaseValue* data, bool is_const);
	static void setupDefaultPrototypes();
};

#endif

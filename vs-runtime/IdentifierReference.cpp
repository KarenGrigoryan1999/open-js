#include "string"
#include "IdentifierReference.h"
#include "Token.h"
#include "IdentifierCollection.h"
#include "EcmaContextStack.h"

using namespace std;

IdentifierReference::IdentifierReference(string name, EcmaEnvironment* block) {
	this->name = name;
	this->block = block;
}

BaseValue* IdentifierReference::eval() {
	if (this->block != NULL) {
		EcmaEnvironment* current = this->block;
		
		while (current != NULL) {
			if (current->records == NULL) {
				current = current->outer;
				continue;
			}
			try {
				return current->records->GetLocal(name);
			}
			catch (BaseException* error_message) {
				current = current->outer;
			}
		}
	}
	return IdentifierCollection::GetGlobal(this->name);
}
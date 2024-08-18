#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "BaseValue.h"
#include "IdentifierCollection.h"

using namespace std;

class EcmaEnvironment {
public:
    IdentifierCollection* records;
    EcmaEnvironment* outer;
};
#endif
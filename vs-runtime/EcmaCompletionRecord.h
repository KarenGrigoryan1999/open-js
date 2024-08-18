#ifndef COMPLETION_RECORD_H
#define COMPLETION_RECORD_H

#include "BaseValue.h"

enum class EcmaCompletionType {
    Normal,
    Break,
    Continue,
    Return,
    Throw
};

struct EcmaCompletionRecord {
    EcmaCompletionType type;
    BaseValue* value;
};
#endif
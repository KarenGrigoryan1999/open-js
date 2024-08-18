#ifndef BLOCK_STATEMENT_H
#define BLOCK_STATEMENT_H

#include "EcmaEnvironment.h"
#include "BlockBaseStatement.h"

using namespace std;

class BlockStatement : public BlockBaseStatement
{
public:
	BlockStatement(EcmaEnvironment* statement) : BlockBaseStatement(statement) {};
};

#endif
#pragma once
#include "TTable.h"

class THashTable : public TTable {
protected:
	virtual int HashFunc(TKey key) const {
		return key;
	}
};
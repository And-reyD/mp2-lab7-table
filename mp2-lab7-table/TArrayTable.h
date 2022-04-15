#pragma once
#include "TTable.h"

class TArrayTable : public TTable {
protected:
	TRecord* arr;
	int size, curr;
public:
	TArrayTable(int _size = 10);
	~TArrayTable();

	bool IsFull();

	TKey GetKey();
	TValue GetValue();

	void Reset();
	void GoNext();
	bool IsEnd();
};

TArrayTable::TArrayTable(int _size) {
	size = _size;
	arr = new TRecord[size];
	curr = -1;
}

TArrayTable::~TArrayTable() {
	delete[] arr;
}

bool TArrayTable::IsFull() {
	return DataCount == size;
}

TKey TArrayTable::GetKey() {
	return arr[curr].key;
}

TValue TArrayTable::GetValue() {
	return arr[curr].val;
}

void TArrayTable::Reset() {
	curr = 0;
}

void TArrayTable::GoNext() {
	curr++;
}

bool TArrayTable::IsEnd() {
	return curr == DataCount;
}